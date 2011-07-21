// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2011 RobotCub Consortium, European Commission FP6 Project IST-004370
 * Authors: Rea Francesco
 * email:   francesco.rea@iit.it
 * website: www.robotcub.org 
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */


#include "asvBiasInterface.h"

#include <yarp/os/Property.h> 
#include <yarp/os/Network.h> 
#include <yarp/os/BufferedPort.h>
#include <yarp/os/ResourceFinder.h>

#include <string.h>
#include <sstream>


#define COMMAND_VOCAB_SET VOCAB3('s','e','t')
#define COMMAND_VOCAB_GET VOCAB3('g','e','t')
#define COMMAND_VOCAB_RIGHT VOCAB4('r','i','g','h')
#define COMMAND_VOCAB_LEFT VOCAB4('l','e','f','t')

#define SYTH_DEFAULT_VALUE    8053457
#define SYTA_DEFAULT_VALUE    8053457
#define SYPA_DEFAULT_VALUE    8053457
#define SYPH_DEFAULT_VALUE    8053457
#define TBP_DEFAULT_VALUE     8053457
#define CDR_DEFAULT_VALUE     8053457
#define CDS_DEFAULT_VALUE     8053457
#define CDP_DEFAULT_VALUE     8053457
#define RPX_DEFAULT_VALUE     8053457
#define RPY_DEFAULT_VALUE     8053457
#define IFR_DEFAULT_VALUE     8053457
#define IFT_DEFAULT_VALUE     8053457
#define IFL_DEFAULT_VALUE     8053457
#define CDOF_DEFAULT_VALUE    8053457
#define SYPW_DEFAULT_VALUE    8053457
#define SYW_DEFAULT_VALUE     8053457
#define CDON_DEFAULT_VALUE    8053457
#define CDD_DEFAULT_VALUE     8053457
#define EMCH_DEFAULT_VALUE    8053457
#define EMCT_DEFAULT_VALUE    8053457
#define CDI_DEFAULT_VALUE     8053457
#define CDRG_DEFAULT_VALUE    8053457
#define SELF_DEFAULT_VALUE    8053457
#define FOLL_DEFAULT_VALUE    8053457
#define ARBP_DEFAULT_VALUE    8053457
#define EMVL_DEFAULT_VALUE    8053457
#define CDC_DEFAULT_VALUE     8053457
#define EMVH_DEFAULT_VALUE    8053457
#define I2V_DEFAULT_VALUE     8053457


using namespace yarp::os;
using namespace std;

GtkWidget *mainWindow=0;
static GdkPixbuf *frame = NULL;

BufferedPort<yarp::sig::FlexImage> *ptr_inputPort=0;


std::string* command;   //reference to the string refering to the last command to send

int _frameN;            //Frame Number
bool _savingSet;        //Save Set of Images mode
// 
//yarp::os::BufferedPort<yarp::os::Bottle> *_pOutPort=0; //Output Point Port
Port* _pOutPort=0;
yarp::os::Bottle _outBottle;//Output Bottle Container
pgmOptions _options; //option set by the config file

GtkWidget *saveSingleDialog;
GtkWidget *saveSetDialog;
GtkWidget *menubar;
GtkWidget *fileMenu, *imageMenu, *helpMenu;
GtkWidget *fileItem, *helpItem;
GtkWidget *fileSingleItem, *fileSetItem, *fileQuitItem;
GtkWidget *imageSizeItem, *imageRatioItem, *imageFreezeItem, *imageFramerateItem;
GtkWidget *synchroDisplayItem;
GtkWidget *helpAboutItem;
// StatusBar
GtkWidget *statusbar;
GtkWidget *fpsStatusBar;
GtkWidget *fpsStatusBar2;

guint timeout_ID=0;
guint timeout_update_ID=0;

ViewerResources _resources;


static void createObjects() {
    command=new string("");
}

static void deleteObjects() {
    /*if (ptr_inputPort!=0)
        delete ptr_inputPort;
    if (ptr_portCallback!=0)
        delete ptr_portCallback;*/
    delete command;
}




//-------------------------------------------------
// Main Window Callbacks
//-------------------------------------------------
/**
* usual callback function 
*/
static void callback( GtkWidget *widget,gpointer   data ){
    printf ("Hello again - %s was pressed \n", (char *) data);
    
    if(!strcmp((char *)data,"Buttonl")){
        printf("Button1");
        command->assign("set b1");
    }
    if(!strcmp((char *)data,"Button2")){
        printf("Button2");
        command->assign("set b2");
    }
    if(!strcmp((char *)data,"Button3")){
        printf("Button3");
        command->assign("set b3");
    }
    if(!strcmp((char *)data,"Button4")){
        printf("Button4");
        command->assign("set b4");
    }
    if(!strcmp((char *)data,"Button5")){
        printf("b5");
        command->assign("set ipp");
    }

}



//-------------------------------------------------
// Call Backs Left
//-------------------------------------------------
static void cb_digits_SynThr( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        Bottle in;
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYTH);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_SynTau( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYTA);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_SynPxlTau( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYPA);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_SynPxlThr( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYPH);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_testPbias( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_TPB);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_CDRefr( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDR);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_CDSf( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDS);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_ReqPuX( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_RPX);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_ReqPuY( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_RPY);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_IFRf( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_IFR);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}

static void cb_digits_IFThr( GtkAdjustment *adj ) {
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_IFT);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
}



static void cb_digits_IFLk( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_IFL);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_CDOffThr( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDOF);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_SynPxlW( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYPW);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_SynW( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SYW);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_CDOnThr( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDON);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_CDDiff( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDD);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_EMCompH( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_EMCH);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_EMCompT( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_EMCT);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_CDIoff( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDI);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}


static void cb_digits_CDRGnd( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDRG);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_Self( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_SELF);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_Foll( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_FOLL);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_ArbPd( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_ARBP);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_EMVrefL( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_EMVL);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_CDCas( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_CDC);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_EMVrefH( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_EMVH);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void cb_digits_I2V( GtkAdjustment *adj ) {
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SET);
        bot.addVocab(COMMAND_VOCAB_I2V);
        bot.addInt((int) adj->value);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

// ----------------- CALL BACK RIGHT -------------------------------------------



// ---------------------------------------------------------------------------------


static void callbackSaveButton( GtkWidget *widget,gpointer data ) {
    printf ("Save button - %s was pressed\n", (char *) data);
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_SAVE);
        bot.addVocab(COMMAND_VOCAB_BIAS);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}

static void callbackProgBiasButton( GtkWidget *widget,gpointer data ) {
    printf ("Prog Bias - %s was pressed\n", (char *) data);
    mutex.wait();
    if (_pOutPort!=NULL) {
        yarp::os::Bottle bot; //= _pOutPort->prepare();
        bot.clear();
        bot.addVocab(COMMAND_VOCAB_PROG);
        if(!strcmp((char*) data,"left")) {
            bot.addVocab(COMMAND_VOCAB_LEFT);
        }
        else {
            bot.addVocab(COMMAND_VOCAB_RIGHT);
        }

        bot.addVocab(COMMAND_VOCAB_BIAS);
        //_pOutPort->Content() = _outBottle;
        Bottle in;
        _pOutPort->write(bot,in);
    }
    mutex.post();
}



gint timeout_update_CB(gpointer data) {
    //portFpsData.getStats(av, min, max);
    //portFpsData.reset();
    gchar *msg;
    //gdk_threads_enter();
    msg=g_strdup_printf("selectiveAttentionInterface");
    updateStatusbar(fpsStatusBar, msg);
    g_free(msg);
    //displayFpsData.getStats(av, min, max);
    //displayFpsData.reset();
    
    //periodToFreq(av, min, max, avHz, minHz, maxHz);

    //msg=g_strdup_printf("Display: %.1f (min:%.1f max:%.1f) fps", avHz, minHz, maxHz);
    //updateStatusbar(fpsStatusBar2, msg);
    //g_free(msg);
    //gdk_threads_leave();
    return TRUE;
}

gint timeout_CB (gpointer data) {
    c++;
    return TRUE;
}


gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data ) {
    // If you return FALSE in the "delete_event" signal handler,
    // GTK will emit the "destroy" signal. Returning TRUE means
    // you don't want the window to be destroyed.
    // This is useful for popping up 'are you sure you want to quit?'
    // type dialogs. 
    cleanExit();
    return TRUE;
}

gint expose_CB (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
    return TRUE;
}

static gboolean configure_event( GtkWidget *widget, GdkEventConfigure *event ) {
   _resources.configure(widget, 
        widget->allocation.width,
        widget->allocation.height);
  return TRUE;
}
gint menuFileQuit_CB(GtkWidget *widget, gpointer data) {
    cleanExit();
    return TRUE;
}

gint menuHelpAbout_CB(GtkWidget *widget, gpointer data) {
#if GTK_CHECK_VERSION(2,6,0)
    const gchar *authors[] = 
        {
            "Yarp developers",
            NULL
        };
    const gchar *license =
        "Released under the terms of the LGPLv2.1 or later, see LGPL.TXT\n"
        "The complete license description is contained in the\n"
        "COPYING file included in this distribution.\n"
        "Please refer to this file for complete\n"
        "information about the licensing of YARP.\n"
        "\n"
        "DISCLAIMERS: LICENSOR WARRANTS THAT THE COPYRIGHT IN AND TO THE\n"
        "SOFTWARE IS OWNED BY THE LICENSOR OR THAT THE SOFTWARE IS\n"
        "DISTRIBUTED BY LICENSOR UNDER A VALID CURRENT LICENSE. EXCEPT AS\n"
        "EXPRESSLY STATED IN THE IMMEDIATELY PRECEDING SENTENCE, THE\n"
        "SOFTWARE IS PROVIDED BY THE LICENSOR, CONTRIBUTORS AND COPYRIGHT\n"
        "OWNERS AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED\n"
        "INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
        "FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO\n"
        "EVENT SHALL THE LICENSOR, CONTRIBUTORS OR COPYRIGHT OWNERS BE\n"
        "LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN\n"
        "ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n"
        "ONNECTION WITH THE SOFTWARE.\n";

    gtk_show_about_dialog(GTK_WINDOW(mainWindow),
                          "name", "selAttentionInterface",
                          "version", "1.0",
                          "license", license,
                          "website", "http://sourceforge.net/projects/yarp0",
                          "comments", "Interface for selectiveAttentionEngine",
                          "authors", authors,
                          NULL);
#else
    printf("Missing functionality on older GTK version, sorry\n");
#endif

    return TRUE;
}

gint menuFileSingle_CB(GtkWidget *widget, GdkEventExpose *event, gpointer data) {
    if ( gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(widget)) ) 
        {
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(fileSetItem), FALSE);
            gtk_widget_show_all (saveSingleDialog);
        } 
    else 
        {
            gtk_widget_hide (saveSingleDialog);
        }

    return TRUE;
}
void setTimedMode(guint dT) {
   // ptr_portCallback->mustDraw(false);
    if (timeout_ID!=0)
        gtk_timeout_remove(timeout_ID);
    timeout_ID = gtk_timeout_add (dT, timeout_CB, NULL);
}

void setSynchroMode() {
    gtk_timeout_remove(timeout_ID);
    timeout_ID=0;
    //ptr_portCallback->mustDraw(true);
}

//-------------------------------------------------
// Non Modal Dialogs
//-------------------------------------------------
GtkWidget* createSaveSingleDialog(void) {

    GtkWidget *dialog = NULL;
    GtkWidget *hbox;
    GtkWidget *button;
    dialog = gtk_dialog_new ();
    gtk_window_set_title(GTK_WINDOW(dialog), "Save Snapshot");
    gtk_window_set_modal(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(mainWindow));
    //gtk_window_resize(GTK_WINDOW(dialog), 185, 40);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    //gtk_window_set_default_size(GTK_WINDOW(dialog), 185, 40);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
    gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
    hbox = gtk_hbox_new (TRUE, 8); // parameters (gboolean homogeneous_space, gint spacing);
    button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_widget_set_size_request (GTK_WIDGET(button), 150,50);
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 16); // parameters (GtkBox *box, GtkWidget *child, gboolean expand, gboolean fill, guint padding);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 8); // parameters (GtkBox *box, GtkWidget *child, gboolean expand, gboolean fill, guint padding);
    
    //gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    
    return dialog;
}

GtkWidget* createSaveSetDialog(void) {
    GtkWidget *dialog = NULL;
    dialog = gtk_dialog_new ();
    gtk_window_set_title(GTK_WINDOW(dialog), "Save Image Set");
    gtk_window_set_modal(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(mainWindow));
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    //gtk_window_set_default_size(GTK_WINDOW(dialog), 190, 40);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
    gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
    return dialog;
}

//-------------------------------------------------
// Main Window Menubar
//-------------------------------------------------
GtkWidget* createMenubar(void) {
    GtkWidget *menubar;

    menubar =  gtk_menu_bar_new ();
    GtkWidget *menuSeparator;	
    // Submenus Items on menubar
    fileItem = gtk_menu_item_new_with_label ("File");
    helpItem = gtk_menu_item_new_with_label ("Help");
    // Submenu: File 
    fileMenu = gtk_menu_new();
    fileSingleItem = gtk_check_menu_item_new_with_label ("Save single image..");
    gtk_menu_append( GTK_MENU(fileMenu), fileSingleItem);
    gtk_signal_connect( GTK_OBJECT(fileSingleItem), "toggled", GTK_SIGNAL_FUNC(menuFileSingle_CB), mainWindow);
    fileSetItem = gtk_check_menu_item_new_with_label ("Save a set of images..");
    gtk_menu_append( GTK_MENU(fileMenu), fileSetItem);
    menuSeparator = gtk_separator_menu_item_new();
    gtk_menu_append( GTK_MENU(fileMenu), menuSeparator);
    fileQuitItem = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append( GTK_MENU(fileMenu), fileQuitItem);
    gtk_signal_connect( GTK_OBJECT(fileQuitItem), "activate", GTK_SIGNAL_FUNC(menuFileQuit_CB), mainWindow);
    // Submenu: Image  
    imageMenu = gtk_menu_new();
    
    menuSeparator = gtk_separator_menu_item_new();
    gtk_menu_append( GTK_MENU(imageMenu), menuSeparator);
    menuSeparator = gtk_separator_menu_item_new();
    gtk_menu_append( GTK_MENU(imageMenu), menuSeparator);
    
    // Submenu: Help
    helpMenu = gtk_menu_new();
    helpAboutItem = gtk_menu_item_new_with_label ("About..");
    gtk_menu_append( GTK_MENU(helpMenu), helpAboutItem);
    gtk_signal_connect( GTK_OBJECT(helpAboutItem), "activate", GTK_SIGNAL_FUNC(menuHelpAbout_CB), mainWindow);
    // linking the submenus to items on menubar
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileItem), fileMenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpItem), helpMenu);
    // appending the submenus to the menubar
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), fileItem);
    gtk_menu_item_set_right_justified (GTK_MENU_ITEM (helpItem), TRUE);
    gtk_menu_bar_append(GTK_MENU_BAR(menubar), helpItem);
  
    return menubar;
}

static GtkWidget *xpm_label_box( gchar     *xpm_filename,gchar *label_text ) {
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *image;

    /* Create box for image and label */
    box = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);

    /* Now on to the image stuff */
    if(xpm_filename!=NULL)
        image = gtk_image_new_from_file (xpm_filename);

    /* Create a label for the button */
    label = gtk_label_new (label_text);

    /* Pack the image and label into the box */
    if(xpm_filename!=NULL)
        gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);
    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 3);

    if(xpm_filename!=NULL)
        gtk_widget_show (image);
    gtk_widget_show (label);

    return box;
}

static void scale_set_default_values( GtkScale *scale ) {
    gtk_range_set_update_policy (GTK_RANGE (scale),GTK_UPDATE_CONTINUOUS);
    gtk_scale_set_digits (scale, 2);
    gtk_scale_set_value_pos (scale, GTK_POS_TOP);
    gtk_scale_set_draw_value (scale, TRUE);
}

//-------------------------------------------------
// Main Window Statusbar
//-------------------------------------------------
void updateStatusbar(GtkWidget *statusbar, gchar *msg) {
    GtkStatusbar *sb=GTK_STATUSBAR (statusbar);

    gtk_statusbar_pop (sb, 0); // clear any previous message, underflow is allowed 
    gtk_statusbar_push (sb, 0, msg);
}

//-------------------------------------------------
// Main Window 
//-------------------------------------------------
GtkWidget* createMainWindow(void) {
    
    
    GtkRequisition actualSize;
    GtkWidget* window;
    
    //gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "selectiveAttentionInterface");
    gtk_window_set_default_size(GTK_WINDOW (window), 205, 300); 
    gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK (gtk_main_quit),
                      NULL);

    // When the window is given the "delete_event" signal (this is given
    // by the window manager, usually by the "close" option, or on the
    // titlebar), we ask it to call the delete_event () function
    // as defined above. The data passed to the callback
    // function is NULL and is ignored in the callback function.
    //g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    // Box for main window
    GtkWidget *buttonSave, *buttonProgBias;
    GtkWidget *boxButton;
    GtkWidget *box, *box2, *box3, *box4, *box5, *box6;
    box = gtk_vbox_new (FALSE, 0); // parameters (gboolean homogeneous_space, gint spacing);
    gtk_container_add (GTK_CONTAINER (window), box);
    // MenuBar for main window
    menubar = createMenubar();
    gtk_box_pack_start (GTK_BOX (box), menubar, FALSE, TRUE, 0); // parameters (GtkBox *box, GtkWidget *child, gboolean expand, gboolean fill, guint padding);
    //gtk_widget_size_request(menubar, &actualSize);
    // Drawing Area : here the image will be drawed
    //da = gtk_drawing_area_new ();
    //g_signal_connect (da, "expose_event", G_CALLBACK (expose_CB), NULL);
    /*if (_options.outputEnabled == 1)
        {
            g_signal_connect (da, "button_press_event", G_CALLBACK (clickDA_CB), NULL);
            // Ask to receive events the drawing area doesn't normally subscribe to
            gtk_widget_set_events (da, gtk_widget_get_events (da) | GDK_BUTTON_PRESS_MASK);
        }*/
    //gtk_box_pack_start(GTK_BOX(box), da, TRUE, TRUE, 0);
    
    
    //Toolbox area
    //creates the area as collection of port processes sequence
    box2 = gtk_vbox_new (FALSE, 0); // parameters (gboolean homogeneous_space, gint spacing);
    gtk_container_add (GTK_CONTAINER (window), box2);
    GtkWidget *boxButtons;
    GtkWidget *boxSliders;
    boxButtons = gtk_vbox_new (FALSE, 0); // parameters (gboolean homogeneous_space, gint spacing);
    gtk_container_set_border_width (GTK_CONTAINER (boxButtons), 0);
    boxSliders = gtk_hbox_new (TRUE, 0); // parameters (gboolean homogeneous_space, gint spacing);
    gtk_container_set_border_width (GTK_CONTAINER (boxSliders), 0);
    
    //-----SCALE section
    GtkWidget *scrollbar;
    GtkWidget *separator;
    GtkWidget *label;
    //GtkWidget *scale;
    
    
    GtkWidget *hscale, *vscale;

    // value, lower, upper, step_increment, page_increment, page_size 
    adj1 = gtk_adjustment_new (0.0, 0.0, 1.0, 0.01, 1.0, 1.0);
    vscale = gtk_vscale_new (GTK_ADJUSTMENT (adj1));
    scale_set_default_values (GTK_SCALE (vscale));
    gtk_box_pack_start (GTK_BOX (boxSliders), vscale, TRUE, TRUE, 0);
    gtk_widget_show (vscale);

    //separator = gtk_vseparator_new ();
    //gtk_box_pack_start (GTK_BOX (boxSliders), separator, FALSE, FALSE, 0);
    //gtk_widget_show (separator);

    //----------BOX3 SECTION:1
    //box3 is the single area that controls the processing towards the output port
    //every processes sequence has a sequence of checkboxes a label and a button
    box3 = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box3), 0);
    gtk_box_pack_start (GTK_BOX (box2), box3, FALSE, FALSE, 0);
    gtk_widget_show (box3);

    //----------BOX5 SUBSECTION:1
    box5 = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box5), 0);

    label = gtk_label_new ("BIAS LEFT:");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    label = gtk_label_new ("SynThr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj1 = gtk_adjustment_new (SYTH_DEFAULT_VALUE, 0,16777215,10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj1));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj1), "value_changed",
                      G_CALLBACK (cb_digits_SynThr), NULL);
    


    label = gtk_label_new ("SynTau");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    adj2 = gtk_adjustment_new (SYTA_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj2));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj2), "value_changed",
                      G_CALLBACK (cb_digits_SynTau), NULL);

    label = gtk_label_new ("SynPxlTau");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj3 = gtk_adjustment_new (SYPA_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj3));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj3), "value_changed",
                      G_CALLBACK (cb_digits_SynPxlTau), NULL);

    label = gtk_label_new ("SynPxlThr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj3 = gtk_adjustment_new (SYPH_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj3));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj3), "value_changed",
                      G_CALLBACK (cb_digits_SynPxlThr), NULL);


    label = gtk_label_new ("testPBias");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj4 = gtk_adjustment_new (TBP_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj4));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj4), "value_changed",
                      G_CALLBACK (cb_digits_testPbias), NULL);

    label = gtk_label_new ("CDRefr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (CDR_DEFAULT_VALUE, 0,16777215, 10 , 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_CDRefr), NULL);

    label = gtk_label_new ("CDSf");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj6 = gtk_adjustment_new (CDS_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj6));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj6), "value_changed",
                      G_CALLBACK (cb_digits_CDSf), NULL);


    label = gtk_label_new ("ReqPuX");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (RPX_DEFAULT_VALUE, 0,16777215, 10 ,1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_ReqPuX), NULL);

    label = gtk_label_new ("ReqPuY");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (RPY_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_ReqPuY), NULL);


    label = gtk_label_new ("IFRf");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (IFR_DEFAULT_VALUE, 0,16777215,10000, 0, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_IFRf), NULL);

    label = gtk_label_new ("IFThr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (IFT_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_IFThr), NULL);

    label = gtk_label_new ("IFLk");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (IFL_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_IFLk), NULL);

    gtk_box_pack_start (GTK_BOX (box3), box5, FALSE, FALSE, 0);
    gtk_widget_show (box5);

    separator = gtk_vseparator_new ();
    gtk_box_pack_start (GTK_BOX (box3), separator, FALSE, TRUE, 10);
    gtk_widget_show (separator);

    // ------------ BOX 5 SUBSECTION 2
    box5 = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box5), 0);

    label = gtk_label_new ("BIAS :");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    label = gtk_label_new ("CDOffThr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj1 = gtk_adjustment_new (CDOF_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj1));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj1), "value_changed",
                      G_CALLBACK (cb_digits_CDOffThr), NULL);
    


    label = gtk_label_new ("SynPxlW");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    adj2 = gtk_adjustment_new (SYPW_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj2));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj2), "value_changed",
                      G_CALLBACK (cb_digits_SynPxlW), NULL);

    label = gtk_label_new ("SynW");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj3 = gtk_adjustment_new (SYW_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj3));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj3), "value_changed",
                      G_CALLBACK (cb_digits_SynW), NULL);

    label = gtk_label_new ("CDOnThr");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj3 = gtk_adjustment_new (CDON_DEFAULT_VALUE, 0,16777215,10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj3));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj3), "value_changed",
                      G_CALLBACK (cb_digits_CDOnThr), NULL);


    label = gtk_label_new ("CDDiff");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj4 = gtk_adjustment_new (CDD_DEFAULT_VALUE, 0.0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj4));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj4), "value_changed",
                      G_CALLBACK (cb_digits_CDDiff), NULL);

    label = gtk_label_new ("EMCompH");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (EMCH_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_EMCompH), NULL);

    label = gtk_label_new ("EMCompT");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj6 = gtk_adjustment_new (EMCT_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj6));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj6), "value_changed",
                      G_CALLBACK (cb_digits_EMCompT), NULL);


    label = gtk_label_new ("CDIoff");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (CDI_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_CDIoff), NULL);

    label = gtk_label_new ("CDRGnd");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (CDR_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_CDRGnd), NULL);


    label = gtk_label_new ("self");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (SELF_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_Self), NULL);

    label = gtk_label_new ("Foll");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (FOLL_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_Foll), NULL);

    label = gtk_label_new ("ArbPd");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (ARBP_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_ArbPd), NULL);

    label = gtk_label_new ("EMVrefL");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (EMVL_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_EMVrefL), NULL);

    label = gtk_label_new ("CDCas");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (CDC_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_CDCas), NULL);

    label = gtk_label_new ("EMVrefH");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (EMVH_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_EMVrefH), NULL);

    label = gtk_label_new ("I2V");
    gtk_box_pack_start (GTK_BOX (box5), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adj5 = gtk_adjustment_new (I2V_DEFAULT_VALUE, 0,16777215, 10, 1000, 0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj5));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box5), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adj5), "value_changed",
                      G_CALLBACK (cb_digits_I2V), NULL);


    gtk_box_pack_start (GTK_BOX (box3), box5, FALSE, FALSE, 0);
    gtk_widget_show (box5);

    separator = gtk_vseparator_new ();
    gtk_box_pack_start (GTK_BOX (box3), separator, FALSE, TRUE, 10);
    gtk_widget_show (separator);

    //----------BOX6 SUBSECTION:3
    box6 = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box6), 0);

    label = gtk_label_new ("FUNCTIONS:");
    gtk_box_pack_start (GTK_BOX (box6), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    
    buttonProgBias = gtk_button_new ();
    g_signal_connect (G_OBJECT (buttonProgBias), "clicked", G_CALLBACK (callbackProgBiasButton),(gpointer) "left");
    boxButton = xpm_label_box (NULL, (gchar*) "ProgBias Left");
    gtk_widget_show (boxButton);
    gtk_container_add (GTK_CONTAINER (buttonProgBias), boxButton);
    gtk_container_add (GTK_CONTAINER (buttonProgBias), box6);
    gtk_widget_show (buttonProgBias);
    gtk_box_pack_start (GTK_BOX (box6), buttonProgBias, FALSE, FALSE, 10);
    gtk_widget_show (buttonProgBias);
    
    buttonProgBias = gtk_button_new ();
    g_signal_connect (G_OBJECT (buttonProgBias), "clicked", G_CALLBACK (callbackProgBiasButton),(gpointer) "right");
    boxButton = xpm_label_box (NULL,  (gchar*)"ProgBias Right");
    gtk_widget_show (boxButton);
    gtk_container_add (GTK_CONTAINER (buttonProgBias), boxButton);
    gtk_container_add (GTK_CONTAINER (buttonProgBias), box6);
    gtk_widget_show (buttonProgBias);
    gtk_box_pack_start (GTK_BOX (box6), buttonProgBias, FALSE, FALSE, 10);
    gtk_widget_show (buttonProgBias);
    
    
    
    buttonSave = gtk_button_new ();
    g_signal_connect (G_OBJECT (buttonSave), "clicked", G_CALLBACK (callbackSaveButton), NULL);
    boxButton = xpm_label_box (NULL,  (gchar*)"Save");
    gtk_widget_show (boxButton);
    gtk_container_add (GTK_CONTAINER (buttonSave), boxButton);
    gtk_container_add (GTK_CONTAINER (buttonSave), box6);
    gtk_box_pack_start (GTK_BOX (box6), buttonSave, FALSE, FALSE, 10);
    gtk_widget_show (buttonSave);
    


    gtk_box_pack_start (GTK_BOX (box3), box6, FALSE, FALSE, 0);
    gtk_widget_show (box6);

    /*
    label = gtk_label_new ("Processing Options:");
    gtk_box_pack_start (GTK_BOX (box3), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    */


    scrollbar = gtk_hscrollbar_new (GTK_ADJUSTMENT (adj1));
    // Notice how this causes the scales to always be updated
    // continuously when the scrollbar is moved 
    //gtk_range_set_update_policy (GTK_RANGE (scrollbar), 
    //                             GTK_UPDATE_CONTINUOUS);
    //gtk_box_pack_start (GTK_BOX (box3), scrollbar, TRUE, TRUE, 0);
    //gtk_widget_show (scrollbar;)

    //-----Check Buttons
    box4=  gtk_vbox_new (FALSE, 0);
    
    gtk_box_pack_start (GTK_BOX (box3), box4, TRUE, TRUE, 0);
    gtk_widget_show (box4);

    //-----box4
    box4=  gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box3), box4, TRUE, TRUE, 0);
    gtk_widget_show (box4);
    //---box 4


    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box2), separator, FALSE, TRUE, 10);
    gtk_widget_show (separator);

    /*
    label = gtk_label_new ("coefficient motion");
    gtk_box_pack_start (GTK_BOX (box2), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    adjMotion = gtk_adjustment_new (0.5, 0.0,1.0,0.01, 0.0, 0.0);
    hscale = gtk_hscale_new (GTK_ADJUSTMENT (adjMotion));
    gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
    scale_set_default_values (GTK_SCALE (hscale));
    gtk_box_pack_start (GTK_BOX (box2), hscale, TRUE, TRUE, 0);
    gtk_widget_show (hscale);
    g_signal_connect (G_OBJECT (adjMotion), "value_changed",
                      G_CALLBACK (cb_digits_scaleMotion), NULL);
                      */
    
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box2), separator, FALSE, TRUE, 10);
    gtk_widget_show (separator);


    //gtk_container_add (GTK_CONTAINER (box2), boxSliders);
    gtk_box_pack_start(GTK_BOX(box), box2,FALSE,FALSE, 10);
    // StatusBar for main window
    statusbar = gtk_statusbar_new ();
    //updateStatusbar(GTK_STATUSBAR (statusbar));
    gtk_box_pack_start (GTK_BOX (box), statusbar, FALSE, TRUE, 0);
    gtk_widget_size_request(statusbar, &actualSize);
    //_occupiedHeight += 2*(actualSize.height);*/

    frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, 320, 240);
    // TimeOut used to refresh the screen
    timeout_ID = gtk_timeout_add (100, timeout_CB, NULL);

    mainWindow=window;

    return window;
}

void configure(yarp::os::ResourceFinder rf){
    /* Process all parameters from both command-line and .ini file */
    /* get the module name which will form the stem of all module port names */
    _options.portName      = rf.check("name", 
                           Value("/asvBiasInterface"), 
                           "module name (string)").asString();
    _options.posX      = rf.check("x", 
                           Value(100), 
                           "module pos x (int)").asInt();
    _options.posY      = rf.check("y", 
                           Value(100), 
                           "module pos y (int)").asInt();

}

void setOptionsToDefault() {
    // Options defaults
    _options.refreshTime = 100;
    _options.outputEnabled = 0;
    _options.windWidth = 300;
    _options.windHeight = 300;
    _options.posX = 100;
    _options.posY = 100;
    _options.saveOnExit = 0;
}

bool openPorts() {
    //_pOutPort = new yarp::os::BufferedPort<yarp::os::Bottle>;
    _pOutPort=new Port;
    _options.portName+="/command:o";
    bool ok = _pOutPort->open(_options.portName.c_str());
    if (ok) {
        g_print("Port registration succeed!\n");
    }
    else {
            g_print("ERROR: Port registration failed.\nQuitting, sorry.\n");
            return false;
    }
    return true;
}

void closePorts() {
    _pOutPort->close();
    bool ok = true;
    if  (ok)
        printf("Port unregistration succeed!\n");
    else 
        printf("ERROR: Port unregistration failed.\n");
    delete _pOutPort;
    _pOutPort = NULL;

}

void cleanExit() {
    if (timeout_ID!=0)
        g_source_remove (timeout_ID);
    timeout_ID = 0;
    
    g_source_remove(timeout_update_ID);
    timeout_update_ID=0;

    gtk_main_quit ();
}

//-------------------------------------------------
// Main
//-------------------------------------------------
#undef main //ace leaves a "main" macro defined

int myMain(int argc, char* argv[]) {
    yarp::os::Network yarp;

    
    //initialize threads in gtk, copied almost verbatim from
    // http://library.gnome.org/devel/gdk/unstable/gdk-Threads.htm
    //g_thread_init (NULL);
    //gdk_threads_init ();
    //gdk_threads_enter ();
    createObjects();
    _frameN = 0;
    timeout_ID = 0;
    setOptionsToDefault();

    yarp::os::ResourceFinder* rf;
    rf=new ResourceFinder();
    rf->setVerbose(true);
    rf->setDefaultConfigFile("asvBiasInterface.ini"); //overridden by --from parameter
    rf->setDefaultContext("eMorphApp/conf");   //overridden by --context parameter
    rf->configure("ICUB_ROOT", argc, argv);
    configure(*rf);
    
    // Parse command line parameters, do this before
    // calling gtk_init(argc, argv) otherwise weird things 
    // happens
    if (!openPorts())
        goto exitRoutine;

    // This is called in all GTK applications. Arguments are parsed
    // from the command line and are returned to the application.
    gtk_init (&argc, &argv);

    // create a new window
    mainWindow = createMainWindow();
    
    // Non Modal Dialogs
#if GTK_CHECK_VERSION(2,6,0)
    saveSingleDialog = createSaveSingleDialog();
    saveSetDialog = createSaveSetDialog();
#else
    printf("Functionality omitted for older GTK version\n");
#endif
    // Shows all widgets in main Window
    gtk_widget_show_all (mainWindow);
    gtk_window_move(GTK_WINDOW(mainWindow), _options.posX, _options.posY);
    // All GTK applications must have a gtk_main(). Control ends here
    // and waits for an event to occur (like a key press or
    // mouse event).

    //ptr_portCallback->attach(&_resources);
    //ptr_portCallback->attach(&portFpsData);
    //ptr_inputPort->useCallback(*ptr_portCallback);

    if (_options.synch)
    {
        setSynchroMode();
        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(synchroDisplayItem), true);
    }
    else
    {
        setTimedMode(_options.refreshTime);
        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(synchroDisplayItem), false);
    }
    gtk_main ();

exitRoutine:
    // leave critical section here. From example
    // http://library.gnome.org/devel/gdk/unstable/gdk-Threads.htm
    //gdk_threads_leave ();

    closePorts();
    deleteObjects();
    return 0;
}

#ifdef YARP_WIN32_NOCONSOLE
#include <windows.h>
// win32 non-console applications define WinMain as the
// entry point for the linker
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    return myMain (__argc, __argv);
}
#else
int main(int argc, char* argv[]) {
    return myMain(argc, argv);
}
#endif

void printHelp() {
    g_print("asvBiasInterface usage:\n");
    g_print("--name: input port name (default: /selAttentionInterface)\n");
}
