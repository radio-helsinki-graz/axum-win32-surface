//---------------------------------------------------------------------------

#ifndef EQWindowDlgH
#define EQWindowDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include "EQPanel.h"
#include "Knob.h"
#include "BitmapButton.h"
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <jpeg.hpp>
#include "ATImage.hpp"
//---------------------------------------------------------------------------
class TEQWindowDialog : public TForm
{
__published:    // IDE-managed Components
  TATImage *BackgroundImage;
   TEQPanel *EQWindow;
  TLabel *EQ1BandwidthLabel;
  TLabel *EQ2BandwidthLabel;
  TLabel *EQ3BandwidthLabel;
  TLabel *EQ3FrequencyLabel;
  TLabel *EQ3LevelLabel;
  TLabel *EQ2FrequencyLabel;
  TLabel *EQ2LevelLabel;
  TLabel *EQ1LevelLabel;
  TLabel *EQ1FrequencyLabel;
  TKnob *EQ1BandwidthKnob;
  TKnob *EQ2BandwidthKnob;
  TKnob *EQ3BandwidthKnob;
  TKnob *EQ3FrequencyKnob;
  TKnob *EQ3LevelKnob;
  TKnob *EQ2FrequencyKnob;
  TKnob *EQ2LevelKnob;
  TKnob *EQ1LevelKnob;
  TKnob *EQ1FrequencyKnob;
  TPopupMenu *EQTypePopupMenu;
  TMenuItem *LowShelvingMenuItem;
  TMenuItem *HighShelvingItem;
  TLabel *EQ4BandwidthLabel;
  TLabel *EQ5BandwidthLabel;
  TLabel *EQ6BandwidthLabel;
  TKnob *EQ6BandwidthKnob;
  TKnob *EQ5BandwidthKnob;
  TKnob *EQ4BandwidthKnob;
  TLabel *EQ4FrequencyLabel;
  TLabel *EQ5FrequencyLabel;
  TLabel *EQ6FrequencyLabel;
  TKnob *EQ6FrequencyKnob;
  TKnob *EQ5FrequencyKnob;
  TKnob *EQ4FrequencyKnob;
  TLabel *EQ4LevelLabel;
  TLabel *EQ5LevelLabel;
  TLabel *EQ6LevelLabel;
  TKnob *EQ6LevelKnob;
  TKnob *EQ5LevelKnob;
  TKnob *EQ4LevelKnob;
  TLabel *EQ1TypeLabel;
  TLabel *EQ2TypeLabel;
  TLabel *EQ3TypeLabel;
  TLabel *EQ4TypeLabel;
  TLabel *EQ5TypeLabel;
  TLabel *EQ6TypeLabel;
  TMenuItem *PeakingBellMenuItem;
  TMenuItem *LowPassFilterMenuItem;
  TMenuItem *HighPassFilterMenuItem;
  TMenuItem *BandPassFilterMenuItem;
  TMenuItem *NotchMenuItem;
  TMenuItem *OffMenuItem;
  TATImage *EQImage;
  TATImage *EQOnImage;
  TATImage *EQOffImage;
  TLabel *EQOnLabel;
   void __fastcall EQWindowMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall EQWindowMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall EQWindowMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall EQLevelKnobMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall EQFrequencyKnobMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
  void __fastcall EQBandwidthKnobMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
  void __fastcall LowShelvingMenuItemClick(TObject *Sender);
  void __fastcall HighShelvingItemClick(TObject *Sender);
  void __fastcall LowPassFilterMenuItemClick(TObject *Sender);
  void __fastcall HighPassFilterMenuItemClick(TObject *Sender);
  void __fastcall BandPassFilterMenuItemClick(TObject *Sender);
  void __fastcall NotchMenuItemClick(TObject *Sender);
  void __fastcall OffMenuItemClick(TObject *Sender);
  void __fastcall PeakingBellMenuItemClick(TObject *Sender);
  void __fastcall EQOnLabelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall EQOnLabelButtonMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall FormCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall EQLevelKnobDblClick(TObject *Sender);
  void __fastcall EQFrequencyKnobDblClick(TObject *Sender);
  void __fastcall EQBandwidthKnobDblClick(TObject *Sender);
private:    // User declarations
  EQAnchor DragEQ;
  int ControlOriginalTop[1024];
  int ControlOriginalLeft[1024];
  int ControlOriginalHeight[1024];
  int ControlOriginalWidth[1024];
  int LabelFontSize;
public:     // User declarations
   int ModuleNr;
   __fastcall TEQWindowDialog(TComponent* Owner);
   TControl *FindFormControl(char *Name);
   int MaximalFontSizeToLabelExtents(TLabel *DisplayLabel, int Percent);
   void CalculateFontSizes();
};
//---------------------------------------------------------------------------
extern PACKAGE TEQWindowDialog *EQWindowDialog;
//---------------------------------------------------------------------------
#endif
