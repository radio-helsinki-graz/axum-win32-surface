object SurfaceSelectForm: TSurfaceSelectForm
  Left = 465
  Top = 253
  Width = 301
  Height = 332
  Caption = 'Surface select'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 132
    Top = 265
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 0
  end
  object Button2: TButton
    Left = 212
    Top = 265
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object TreeView1: TTreeView
    Left = 8
    Top = 32
    Width = 273
    Height = 225
    Indent = 19
    TabOrder = 2
  end
end
