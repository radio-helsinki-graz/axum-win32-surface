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
//---------------------------------------------------------------------------
class TEQWindowDialog : public TForm
{
__published:	// IDE-managed Components
   TPanel *StatusPanel;
   TImage *EQWindowBackgroundImage;
   TStatusBar *StatusLine;
   TPanel *ButtonPanel;
   TButton *OkButton;
   TEQPanel *EQWindow;
  TStaticText *EQ1BandwidthLabel;
  TStaticText *EQ2BandwidthLabel;
  TStaticText *EQ3BandwidthLabel;
  TStaticText *EQ3FrequencyLabel;
  TStaticText *EQ3LevelLabel;
  TStaticText *EQ2FrequencyLabel;
  TStaticText *EQ2LevelLabel;
  TStaticText *EQ1LevelLabel;
  TStaticText *EQ1FrequencyLabel;
  TKnob *EQ1BandwidthKnob;
  TKnob *EQ2BandwidthKnob;
  TKnob *EQ3BandwidthKnob;
  TKnob *EQ3FrequencyKnob;
  TKnob *EQ3LevelKnob;
  TKnob *EQ2FrequencyKnob;
  TKnob *EQ2LevelKnob;
  TKnob *EQ1LevelKnob;
  TKnob *EQ1FrequencyKnob;
   TBitmapButton *EQOn1BitmapButton;
  TPopupMenu *EQTypePopupMenu;
  TMenuItem *LowShelvingMenuItem;
  TMenuItem *HighShelvingItem;
  TStaticText *EQ4BandwidthLabel;
  TStaticText *EQ5BandwidthLabel;
  TStaticText *EQ6BandwidthLabel;
  TKnob *EQ6BandwidthKnob;
  TKnob *EQ5BandwidthKnob;
  TKnob *EQ4BandwidthKnob;
  TStaticText *EQ4FrequencyLabel;
  TStaticText *EQ5FrequencyLabel;
  TStaticText *EQ6FrequencyLabel;
  TKnob *EQ6FrequencyKnob;
  TKnob *EQ5FrequencyKnob;
  TKnob *EQ4FrequencyKnob;
  TStaticText *EQ4LevelLabel;
  TStaticText *EQ5LevelLabel;
  TStaticText *EQ6LevelLabel;
  TKnob *EQ6LevelKnob;
  TKnob *EQ5LevelKnob;
  TKnob *EQ4LevelKnob;
  TStaticText *EQ1TypeLabel;
  TStaticText *EQ2TypeLabel;
  TStaticText *EQ3TypeLabel;
  TStaticText *EQ4TypeLabel;
  TStaticText *EQ5TypeLabel;
  TStaticText *EQ6TypeLabel;
  TMenuItem *PeakingBellMenuItem;
  TMenuItem *LowPassFilterMenuItem;
  TMenuItem *HighPassFilterMenuItem;
  TMenuItem *BandPassFilterMenuItem;
  TMenuItem *NotchMenuItem;
  TMenuItem *OffMenuItem;
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
  void __fastcall EQOn1BitmapButtonMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall EQOn1BitmapButtonMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
    EQAnchor DragEQ;
public:		// User declarations
   int ModuleNr;
   __fastcall TEQWindowDialog(TComponent* Owner);
   TControl *FindFormControl(char *Name);
};
//---------------------------------------------------------------------------
extern PACKAGE TEQWindowDialog *EQWindowDialog;
//---------------------------------------------------------------------------
#endif
