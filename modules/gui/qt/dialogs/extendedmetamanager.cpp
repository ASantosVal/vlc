/*****************************************************************************
 * extendedmetamanager.cpp : Extended Metadata Manager dialogs
 ****************************************************************************
 * Copyright (C) 2017 Asier Santos Valcárcel
 * Authors: Asier Santos Valcarcel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "dialogs/extendedmetamanager.hpp"
#include "components/interface_widgets.hpp"     // CoverArtLabelExt
#include "dialogs/fingerprintdialog.hpp"        // fingerprinting dialog
#include "adapters/chromaprint.hpp"             // fingerprinting adapter (no UI)

#include <QMessageBox>

#define UNUSED(x) (void)(x) //FIXME: delete this. Unused variable warning removal

ExtMetaManagerDialog::ExtMetaManagerDialog( intf_thread_t *_p_intf)
               : QVLCDialog( (QWidget*)_p_intf->p_sys->p_mi, _p_intf )
{
    msg_Dbg( p_intf, "[EMM_Dialog] Initializing" );
    configureWindow();
    initializeWorkspace();

    //TODO: refactor the following line
    connect(&fingreprinterMapper, SIGNAL(mapped(int, bool, input_item_t)), this, SLOT(handleResults(int, bool, input_item_t))); //TODO:This may not be needed

    QVLCTools::restoreWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

ExtMetaManagerDialog::~ExtMetaManagerDialog() {
    msg_Dbg( p_intf, "[EMM_Dialog] Destroying" );
    QVLCTools::saveWidgetPosition( p_intf, "ExtMetaManagerDialog", this );
}

// Override the closing (click X) event
void ExtMetaManagerDialog::closeEvent(QCloseEvent *event) {
    msg_Dbg( p_intf, "[EMM_Dialog] Close event" );
    UNUSED(event); //FIXME: delete this
    close();
}

// Show/hide window
void ExtMetaManagerDialog::toggleVisible() {
    msg_Dbg( p_intf, "[EMM_Dialog] toggleVisible" );
    QVLCDialog::toggleVisible();
    if(isVisible())
        activateWindow();
}

/*----------------------------------------------------------------------------*/
/*-----------------------------Main use cases---------------------------------*/
/*----------------------------------------------------------------------------*/

// Closes the window (and the module itself)
void ExtMetaManagerDialog::close() {
    toggleVisible();
    resetEnvironment();
    msg_Dbg( p_intf, "[EMM_Dialog] Closing" );
}

// Loads files into the table from the current playlist
void ExtMetaManagerDialog::getFromPlaylist() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] getFromPlaylist" );

    if (isClearSelected()){
        resetEnvironment();
    }

    /* We dont want the table to mess things while we update it, so que block
    its signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    /* Lock the playlist so we can work with it */
    playlist_Lock(THEPL);

    /* NOTE: Since the playlist usage is confusing, here is what I understood:
    --The number of elements playing is THEPL->items.i_size-5
    --Items currently playing start on position 8 and finish at sizeOfPlaylist+8 ('playlist_CurrentSize(THEPL)' only returns 0 or 1) */
    int sizeOfPlaylist = THEPL->items.i_size - 5;
    int whereTheLoadedItemsStart = 8;
    int whereTheLoadedItemsFinish = sizeOfPlaylist + 8;

    /* This will be used to show a warning if nothing is loaded */
    bool itemsWereLoaded = false;

    if( sizeOfPlaylist ==0 ) {
        playlist_Unlock(THEPL);
        return;
    }

    input_item_t *p_item;  //This is where each item will be stored

    /* Go through the playlist */
    for(int i = whereTheLoadedItemsStart;  i < whereTheLoadedItemsFinish; i++)
    {
        /* Get the current playlist_item's input_item_t */
        p_item = playlist_ItemGetById(THEPL, i)->p_input;

        /* Evaluate if it's an audio file */
        if (isAudioFile(input_item_GetURI(p_item)))
        {
            /* Add item to the table */
            addTableEntry(p_item);

            itemsWereLoaded = true;
        }
    }

    /* If playlist was empty, show warning */
    if (!itemsWereLoaded)
    {
        launchEmptyPlaylistDialog();
    } else {
        ui.tableWidget_metadata->setCurrentCell(0,1);
        updateArtworkInUI(0,0);
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);

    /* Always unlock the playlist */
    playlist_Unlock(THEPL);
}

// Loads files into the table from a file explorer window
void ExtMetaManagerDialog::getFromFolder() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] getFromFolder" );

    QStringList uris = launchAudioFileSelector();

    /* If no files selected, finish */
    if( uris.isEmpty() ) return;

    /* Clean before changing the workspace */
    if (isClearSelected()){
        resetEnvironment();
    }

    /* We dont want the table to mess things while we update it, so que block
    its signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    foreach( const QString &uri, uris ) {
        if (isAudioFile(uri.toLatin1().constData())) {
            // Get the item from the URI
            input_item_t *p_item = createItemFromURI(uri.toLatin1().constData());

            addTableEntry(p_item); //Add the item to the table
        }
    }

    if (!tableIsEmpty()) {
        /* Select the first cell and update artwork label */
        ui.tableWidget_metadata->setCurrentCell(0,1);
        updateArtworkInUI(0,0);
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);
}

void ExtMetaManagerDialog::initiateMetadataSearch() {
    bool isFastSearch;
    if (ui.checkBox_disableFastSearch->isChecked()) {
        isFastSearch = false;
    } else {
        isFastSearch = true;
    }
    fingerprintTable(isFastSearch);
}

void ExtMetaManagerDialog::saveChanges() {
    msg_Dbg( p_intf, "[EMM_Dialog] saveChanges" );

    input_item_t *temp_item;

    int rows = ui.tableWidget_metadata->rowCount();
    for(int row = 0;  row < rows; row++) {
        if (isRowSelected(row)) {
            temp_item = recoverItemFromRow(row);
            saveItemChanges(temp_item, row);
        }
    }
}

void ExtMetaManagerDialog::discardUnsavedChanges() { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] discardUnsavedChanges" );

    clearTable();
    int workspaceSize = vlc_array_count(workspace);
    for(int i = 0; i < workspaceSize; i++)
    {
        /* Get one item from the "workspace", get it's URI, create a new
        input_item_t and add it to the table (we do this because we want to
        fetch the original metadata again)*/

        input_item_t *p_item_old = (input_item_t*)vlc_array_item_at_index(workspace, i);

        //TODP: this was fixed on another branch
        vlc_array_remove(workspace, i); //We do this to avoid bugs when reloading table

        char *uri_text = input_item_GetURI(p_item_old);
        input_item_t *p_item_reloaded = createItemFromURI(uri_text);

        addTableEntry(p_item_reloaded);
    }
}

/*----------------------------------------------------------------------------*/
/*---------------------------Fingerpinting------------------------------------*/
/*----------------------------------------------------------------------------*/

/* Initiates the fingerprint process for all the table. If "fast" is true, 1st
entry is applied automatically */
void ExtMetaManagerDialog::fingerprintTable( bool isFastSearch ) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] fingerprintTable" );

    input_item_t *temp_item; // This is where the current working item will be

    int totalRowAmount = ui.tableWidget_metadata->rowCount();
    int selectedRowsAmount = countSelectedRows();

    if (selectedRowsAmount == 0)
        return;

    /* Calculate how much the progress bar will advance each step (progressBar
    goes from 0 to 100). Then progress variable is set to 0 and the widget is
    updated */
    progress_unit = 100 / selectedRowsAmount;
    progress = 100 % selectedRowsAmount; //Start with the mod, so it adds 100 at the end
    ui.progressBar_search->setValue(progress);

    /* We don't want the table to mess things while we update it, so que block
    it's signals (this is caused because we edited "multipleItemsChanged"). */
    ui.tableWidget_metadata->blockSignals(true);

    for(int row = 0; row < totalRowAmount; row++)
    {
        if (isRowSelected(row))
        {
            temp_item = recoverItemFromRow(row);
            fingerprintItem(temp_item, isFastSearch);
        }
    }

    /* We have finished, so we unlock all the table's signals. */
    ui.tableWidget_metadata->blockSignals(false);
}

/* Initiates the fingerprint process just for one item. If "fast" is true, 1st
entry is applied automatically */

void ExtMetaManagerDialog::fingerprintItem(input_item_t *p_item, bool isFastSearch) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] fingerprint" );

    if (isFastSearch) {
        //TODO: this creates a new fingerprinter each time, but may not work ( due to reference to items lost/changed)
        t = new (std::nothrow) Chromaprint( p_intf );
        if ( !t )
        {
            return; // Error
        }

        //TODO: this doesn't work
        // /* Prepare the mapping with the row and connect teh button to it.
        // The mapper is used to be able to know from which row is the button is
        // being clicked */
        // fingreprinterMapper.setMapping(t, 1); //TODO: give real values
        // fingreprinterMapper.setMapping(t, "p_item"); //TODO: give real values
        // fingreprinterMapper.setMapping(t, false);  //TODO: give real values
        // CONNECT(t, finished(), this, fingreprinterMapper.map(int, bool, input_item_t *));
        CONNECT(t, finished(), this, handleResults());

        /* Add the item to the finperprinter's queue */
        if ( t )
            t->enqueue( p_item );
    } else {
        launchFingerprinterDialog(p_item);
    }
}


/* When fingerprinter has finished, this method is triggered. It applies the
found metadata and updates the entry on the table */
void ExtMetaManagerDialog::handleResults(/*int row, bool isLast, input_item_t *p_item*/) {
    msg_Err( p_intf, "[EMM_Dialog] handleResults" ); //TODO: delete this or change it to 'debug'

    /* Update the progress bar */
    progress=progress+progress_unit; // Increase the progress
    ui.progressBar_search->setValue(progress); // Update the progressBar

    //TODO: Due to reference loss, the following will give segmentation fault
    // p_r = t->fetchResults();
    //
    // if ( ! p_r )
    // {
    //     return;
    // }
    //
    // if ( vlc_array_count( & p_r->results.metas_array ) == 0 )
    // {
    //     fingerprint_request_Delete( p_r );
    //     p_r = NULL;
    //     return;
    // }

    // /* Apply first option */
    // t->apply( p_r, 0 );

    //TODO:fix this
    /* Update the table with the new info */
    // updateTableEntry(p_item, row);

    //TODO:fix this (when should I delete it? Am I deleting the correct fingerprinter?)
    // if (isLast){
    //     /* Delete the fingerprinter */
    //     if ( t ) delete t;
    //     if ( p_r ) fingerprint_request_Delete( p_r );
    // }
}

void ExtMetaManagerDialog::launchFingerprinterDialog(input_item_t *p_item) {
    FingerprintDialog dialog(this, p_intf, p_item);
    dialog.exec();
}

/*----------------------------------------------------------------------------*/
/*------------------------------Item management-------------------------------*/
/*----------------------------------------------------------------------------*/

input_item_t* ExtMetaManagerDialog::recoverItemFromRow(int row) {
    msg_Dbg( p_intf, "[EMM_Dialog] getItemFromRow" );

    /* Retrieve URI from the wanted row */
    const char * wantedUri = ui.tableWidget_metadata->item(row,COL_PATH)->text().toLocal8Bit().constData();

    /* Get size of "workspace" and travel through it */
    int arraySize = vlc_array_count(workspace);
    for(int i = 0; i < arraySize; i++)
    {
        /* Get item on position "i" */
        input_item_t *p_item = (input_item_t*)vlc_array_item_at_index(workspace, i);

        /* Get uri from that item */
        char * temp_uri = input_item_GetURI(p_item);

        /* Compare it to "wantedUri". If true, means it's que item we wanted, so return it. */
        if (strcmp(temp_uri,wantedUri) == 0){
            return p_item;
        }
    }

    //TODO: return error
}

/* Gets an item from an URI and preparses it (gets it's metadata) */
input_item_t* ExtMetaManagerDialog::createItemFromURI(const char* uri) {
    msg_Dbg( p_intf, "[EMM_Dialog] createItemFromURI" );

    input_item_t *p_item = input_item_New( uri, "" ); //The name is not relevant

    // Preparse the item (get the metadata)
    libvlc_MetadataRequest(THEPL->obj.libvlc, p_item, META_REQUEST_OPTION_SCOPE_ANY,-1, NULL );
    while (!input_item_IsPreparsed(p_item)) ;

    return p_item;
}

bool ExtMetaManagerDialog::isAudioFile(const char* uri) { //TODO: clean this method
    // The following section extracts the extension from the uri
    char extension[255];
    memset(extension, 0, sizeof extension);
    for (size_t i = 0; i < strlen(uri); i++){
        if (uri[i]=='.'){
            memset(extension, 0, sizeof extension);;
        }else{
            int len = strlen(extension);
            extension[len] = uri[i];
            extension[len+1] = '\0';
        }
    }

    char temp[255];
    memset(temp, 0, sizeof temp);

    /* Now look for all the audio extension available */
    for (size_t i = 0; i < strlen(EXTENSIONS_AUDIO); i++){
        /* End of an extension, compare our file's extension to the found one
        and reset temp */
        if (EXTENSIONS_AUDIO[i]=='.')
        {
            /* If our file's extension is valid, return true */
            if (strcmp(extension,temp) == 0)
            {
                return true;
            }

            /* Reset "temp" */
            memset(temp, 0, sizeof temp);
        }
        // If charater is found (not separator), add it to the current extension
        else if (EXTENSIONS_AUDIO[i] != '*' && EXTENSIONS_AUDIO[i] != ';')
        {
            int len = strlen(temp);
            temp[len] = EXTENSIONS_AUDIO[i];
            temp[len+1] = '\0';
        }
    }

    /* If our file's extension is invalid, return false */
    return false;
}

void ExtMetaManagerDialog::saveItemChanges( input_item_t *p_item, int rowFrom) {
    const char *title_text;
    const char *artist_text;
    const char *album_text;
    const char *genre_text;
    const char *trackNum_text;
    const char *publisher_text;
    const char *copyright_text;

    /* ---Extract information form the table---
    getting the reference to one columns info is tricky, because the
    pointer is only valid while the QString (got with text()) hasn't
    changed. This means every time you get info from a new row, you
    loose the pointers to the previous row. That's why here we get the
    info and store it inmediately*/
    title_text = ui.tableWidget_metadata->item(rowFrom,COL_TITLE)->text().toLocal8Bit().constData();
    input_item_SetTitle(p_item, title_text);
    artist_text = ui.tableWidget_metadata->item(rowFrom,COL_ARTIST)->text().toLocal8Bit().constData();
    input_item_SetArtist(p_item, artist_text);
    album_text = ui.tableWidget_metadata->item(rowFrom,COL_ALBUM)->text().toLocal8Bit().constData();
    input_item_SetAlbum(p_item, album_text);
    genre_text = ui.tableWidget_metadata->item(rowFrom,COL_GENRE)->text().toLocal8Bit().constData();
    input_item_SetGenre(p_item, genre_text);
    trackNum_text = ui.tableWidget_metadata->item(rowFrom,COL_TRACKNUM)->text().toLocal8Bit().constData();
    input_item_SetTrackNum(p_item, trackNum_text);
    publisher_text = ui.tableWidget_metadata->item(rowFrom,COL_PUBLISHER)->text().toLocal8Bit().constData();
    input_item_SetPublisher(p_item, publisher_text);
    copyright_text = ui.tableWidget_metadata->item(rowFrom,COL_COPYRIGHT)->text().toLocal8Bit().constData();
    input_item_SetCopyright(p_item, copyright_text);

    // Save the metas on the item
    input_item_WriteMeta( VLC_OBJECT(THEPL), p_item );
}

/*----------------------------------------------------------------------------*/
/*--------------------------Table management----------------------------------*/
/*----------------------------------------------------------------------------*/

/* Modify the table's behavior so when multiple cells are selected, their text
 is changed all at once */
void ExtMetaManagerDialog::multipleItemsChanged( QTableWidgetItem *item ) {
    ui.tableWidget_metadata->blockSignals(true);
    QList<QTableWidgetItem*> selectedItems = ui.tableWidget_metadata->selectedItems();
    foreach(QTableWidgetItem* selectItem, selectedItems)
    {
        selectItem->setText(item->text());
    }
    ui.tableWidget_metadata->blockSignals(false);
}

void ExtMetaManagerDialog::addTableEntry(input_item_t *p_item) {
    msg_Dbg( p_intf, "[EMM_Dialog] addTableEntry" );


    if (itemAlreadyOnLoaded(p_item)){
        return;
    }

    int newRowNumber = addNewRow();
    createCheckboxOnRow(newRowNumber);
    createChangeartworkButtonOnRow(newRowNumber);
    fillRowWithMetadata(p_item, newRowNumber);

    /* Now we append file to our "workspace" */
    vlc_array_append(workspace, p_item); //TODO: clean this
}

void ExtMetaManagerDialog::fillRowWithMetadata(input_item_t *p_item, int row) {
    msg_Dbg( p_intf, "[EMM_Dialog] fillRowWithMetadata" );

    // Get metadata information from item
    char *title_text = input_item_GetTitle(p_item);
    char *artist_text = input_item_GetArtist(p_item);
    char *album_text = input_item_GetAlbum(p_item);
    char *genre_text = input_item_GetGenre(p_item);
    char *trackNum_text = input_item_GetTrackNum(p_item);
    char *publisher_text = input_item_GetPublisher(p_item);
    char *copyright_text = input_item_GetCopyright(p_item);
    char *uri_text = input_item_GetURI(p_item);

    // Fill all the cells with the extracted info
    ui.tableWidget_metadata->setItem(row, COL_TITLE, new QTableWidgetItem( title_text ));
    ui.tableWidget_metadata->setItem(row, COL_ARTIST, new QTableWidgetItem( artist_text ));
    ui.tableWidget_metadata->setItem(row, COL_ALBUM, new QTableWidgetItem( album_text ));
    ui.tableWidget_metadata->setItem(row, COL_GENRE, new QTableWidgetItem( genre_text ));
    ui.tableWidget_metadata->setItem(row, COL_TRACKNUM, new QTableWidgetItem( trackNum_text ));
    ui.tableWidget_metadata->setItem(row, COL_PUBLISHER, new QTableWidgetItem( publisher_text ));
    ui.tableWidget_metadata->setItem(row, COL_COPYRIGHT, new QTableWidgetItem( copyright_text ));
    ui.tableWidget_metadata->setItem(row, COL_PATH, new QTableWidgetItem( uri_text ));
    ui.tableWidget_metadata->item(row, COL_PATH)->setFlags(0); // Make the path not selectable/editable
}

void ExtMetaManagerDialog::clearTable() {
    msg_Dbg( p_intf, "[EMM_Dialog] clearTable" );

    ui.tableWidget_metadata->clearContents();
    ui.tableWidget_metadata->setRowCount(0);
    art_cover->clear();
}

int ExtMetaManagerDialog::countSelectedRows() {
    int totalRowAmount = ui.tableWidget_metadata->rowCount();
    int selectedRowsCount=0;

    for(int row = 0;  row < totalRowAmount; row++) {
        if (isRowSelected(row)){
            selectedRowsCount++;
        }
    }
    return selectedRowsCount;
}

bool ExtMetaManagerDialog::tableIsEmpty() {
    return (ui.tableWidget_metadata->rowCount()>0);
}

bool ExtMetaManagerDialog::isRowSelected(int row) {
    QCheckBox *checkbox = (QCheckBox*) ui.tableWidget_metadata->cellWidget(row,COL_CHECKBOX);
    return checkbox->isChecked();
}

void ExtMetaManagerDialog::createCheckboxOnRow(int row) {
    QCheckBox *checkbox = new QCheckBox ();
    checkbox->setChecked(true);
    checkbox->setToolTip(checkbox_tip);
    ui.tableWidget_metadata->setCellWidget(row, COL_CHECKBOX, checkbox);
}

int ExtMetaManagerDialog::addNewRow() {
    int newRowNumber = ui.tableWidget_metadata->rowCount(); //Last row is always rowCount-1
    ui.tableWidget_metadata->insertRow(newRowNumber);
    return newRowNumber;
}

void ExtMetaManagerDialog::createChangeartworkButtonOnRow(int row) {
    QPushButton *button_changeArtwork = new QPushButton( qtr("Change"), this);
    button_changeArtwork->setToolTip(artworkButton_tip);

    /* Prepare the mapper to be able to know from which row is the button is
    being clicked */
    ButtonSignalMapper.setMapping(button_changeArtwork, row);
    connect(button_changeArtwork, SIGNAL(clicked()), &ButtonSignalMapper, SLOT(map()));

    // Insert the button in the cell
    ui.tableWidget_metadata->setCellWidget(row, COL_ARTWORK, button_changeArtwork );
}

/*----------------------------------------------------------------------------*/
/*----------------------------Artwork management------------------------------*/
/*----------------------------------------------------------------------------*/

/* When a cell on the table is selected, this function changes the Artwork
label's content to the selected item's artwork */
void ExtMetaManagerDialog::updateArtworkInUI(int row, int column) {
    msg_Dbg( p_intf, "[EMM_Dialog] updateArtworkInUI" );
    UNUSED(column); //FIXME: delete this, just avoids a warning
    art_cover->showArtUpdate( recoverItemFromRow(row) );
}

void ExtMetaManagerDialog::changeArtwork(int row) {
    msg_Dbg( p_intf, "[EMM_Dialog] changeArtwork" );

    // Fix to know the row the button is being clicked from and show it's cover
    ui.tableWidget_metadata->setCurrentCell(row, COL_ARTWORK);
    updateArtworkInUI(row, COL_ARTWORK);

    art_cover->setArtFromFile();
}

/*----------------------------------------------------------------------------*/
/*------------------------- UI configuration ---------------------------------*/
/*----------------------------------------------------------------------------*/

void ExtMetaManagerDialog::configureWindow(){
    configureBasicUI();
    configureButtons();
    configureTable();
    configureArtworkLabel();
    configureProgressBar();
    setToolTips();
}

void ExtMetaManagerDialog::configureBasicUI() {
    ui.setupUi( this ); //from de compiled (.h) version of de QT ui (.ui) file
    setWindowFlags( Qt::Tool );
    setWindowRole( "vlc-ext-meta-manager" );
    setWindowTitle( qtr( "Extended Metadata Manager" ) );
}

void ExtMetaManagerDialog::configureTable() {
    setTableEvents();
    setColumnSizes();

    /* Set the mapper's connection to changeArtwork. This is used to be able
    to know from which row the button (added later on each row) it's being
    clicked */
    connect(&ButtonSignalMapper, SIGNAL(mapped(int)), this, SLOT(changeArtwork(int)));

}

void ExtMetaManagerDialog::configureButtons() {
    setButtonIcons();
    setButtonBindings();
}

void ExtMetaManagerDialog::configureArtworkLabel() {
    art_cover = new CoverArtLabelExt( this, p_intf );
    ui.gridLayout_artwork->layout()->addWidget(art_cover);
}

void ExtMetaManagerDialog::configureProgressBar() {
    ui.progressBar_search->setEnabled(false);
}

void ExtMetaManagerDialog::setTableEvents() {
    CONNECT( ui.tableWidget_metadata, cellClicked(int, int), this, updateArtworkInUI(int, int) );
    CONNECT( ui.tableWidget_metadata,  itemChanged(QTableWidgetItem*), this,  multipleItemsChanged(QTableWidgetItem*));
}

void ExtMetaManagerDialog::setColumnSizes() {
    ui.tableWidget_metadata->setColumnWidth(COL_CHECKBOX, 30);
    ui.tableWidget_metadata->setColumnWidth(COL_TITLE, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTIST, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_ALBUM, 200);
    ui.tableWidget_metadata->setColumnWidth(COL_GENRE, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_TRACKNUM, 70);
    ui.tableWidget_metadata->setColumnWidth(COL_PUBLISHER, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_COPYRIGHT, 120);
    ui.tableWidget_metadata->setColumnWidth(COL_ARTWORK, 70);
    ui.tableWidget_metadata->setColumnWidth(COL_PATH, 50);
}

void ExtMetaManagerDialog::setButtonIcons() {
    ui.pushButton_getFromPlaylist->setIcon(QIcon(QPixmap (":/toolbar/playlist") ) );
    ui.pushButton_getFromFolder->setIcon(QIcon(QPixmap (":/type/folder-grey") ) );
    ui.pushButton_help->setIcon(QIcon(QPixmap (":/menu/help") ) );
    ui.pushButton_about->setIcon(QIcon(QPixmap (":/menu/info") ) );
    ui.pushButton_searchNow->setIcon(QIcon(QPixmap (":/ext-meta-manager/search") ) );
    ui.pushButton_saveAll->setIcon(QIcon(QPixmap (":/ext-meta-manager/save") ) );
    ui.pushButton_restoreAll->setIcon(QIcon(QPixmap (":/buttons/playlist/repeat_all") ) );
    ui.pushButton_clearTable->setIcon(QIcon(QPixmap (":/ext-meta-manager/clean") ) );
    ui.pushButton_cancel->setIcon(QIcon(QPixmap (":/menu/exit") ) );
}

void ExtMetaManagerDialog::setButtonBindings() {
    BUTTONACT( ui.pushButton_getFromPlaylist, getFromPlaylist() );
    BUTTONACT( ui.pushButton_getFromFolder, getFromFolder() );
    BUTTONACT( ui.pushButton_help, launchHelpDialog() );
    BUTTONACT( ui.pushButton_about, launchAboutDialog() );
    BUTTONACT( ui.pushButton_searchNow, initiateMetadataSearch() );
    BUTTONACT( ui.pushButton_saveAll, saveChanges() );
    BUTTONACT( ui.pushButton_restoreAll, discardUnsavedChanges() );
    BUTTONACT( ui.pushButton_clearTable, resetEnvironment() );
    BUTTONACT( ui.pushButton_cancel, close() );
}

void ExtMetaManagerDialog::setToolTips() {
    ui.pushButton_getFromPlaylist->setToolTip(getFromPlaylist_tip);
    ui.pushButton_getFromFolder->setToolTip(getFromFolder_tip);
    ui.pushButton_help->setToolTip(help_tip);
    ui.pushButton_about->setToolTip(about_tip);
    ui.pushButton_searchNow->setToolTip(searchNow_tip);
    ui.pushButton_saveAll->setToolTip(saveAll_tip);
    ui.pushButton_restoreAll->setToolTip(restoreAll_tip);
    ui.pushButton_clearTable->setToolTip(clearTable_tip);
    ui.pushButton_cancel->setToolTip(cancel_tip);
    ui.progressBar_search->setToolTip(progressBar_tip);
    ui.checkBox_disableFastSearch->setToolTip(disableFastSearch_tip);
    art_cover->setToolTip(artwork_tip);
}

/*----------------------------------------------------------------------------*/
/*-------------------------------Others---------------------------------------*/
/*----------------------------------------------------------------------------*/

void ExtMetaManagerDialog::resetEnvironment() {
    msg_Dbg( p_intf, "[EMM_Dialog] resetEnvironment" );
    clearTable();
    vlc_array_clear(workspace);
}

void ExtMetaManagerDialog::initializeWorkspace(){
    workspace = new vlc_array_t();
    vlc_array_init(workspace);
}

void ExtMetaManagerDialog::launchHelpDialog() {
    QMessageBox::information(
      this,
      help_dialog_title,
      help_dialog_text );
}

void ExtMetaManagerDialog::launchAboutDialog() {
    QMessageBox::information(
      this,
      about_dialog_title,
      about_dialog_text );
}

void ExtMetaManagerDialog::launchEmptyPlaylistDialog() {
    QMessageBox::information(
      this,
      emptyPlaylist_dialog_title,
      emptyPlaylist_dialog_text );
}

QStringList ExtMetaManagerDialog::launchAudioFileSelector() {
    toggleVisible(); // Hide window
    QStringList uris = THEDP->showSimpleOpen(
        qtr("Open audio files to manage"),
        EXT_FILTER_AUDIO,
        p_intf->p_sys->filepath );
    toggleVisible();// Show window again
    return uris;
}

bool ExtMetaManagerDialog::isClearSelected() {
    return ui.checkBox_cleaTable->isChecked();
}

//TODO: this code may be repeated on other branches
bool ExtMetaManagerDialog::itemAlreadyOnLoaded(input_item_t *new_item) { //TODO: clean this method
    msg_Dbg( p_intf, "[EMM_Dialog] itemAlreadyOnLoaded" );

    /* Get uri from that "new_item" */
    char * wantedUri = input_item_GetURI(new_item);

    input_item_t *p_item;
    char * temp_uri;

    int arraySize = vlc_array_count(workspace);
    for(int i = 0; i < arraySize; i++)
    {
        /* Get item on position "i" */
        p_item = (input_item_t*)vlc_array_item_at_index(workspace, i);

        /* Get uri from that item */
        temp_uri = input_item_GetURI(p_item);

        /* Compare it to "wantedUri". If true, means it's que item is repeated, so return true. */
        if (strcmp(temp_uri,wantedUri) == 0){
            msg_Dbg( p_intf, "[EMM_Dialog] repeated item found, ignoring it." );
            return true;
        }

    }
    return false; //No matches found, so item is not already loaded.
}
