object EthernetAdapterSelectionForm: TEthernetAdapterSelectionForm
  Left = 237
  Top = 182
  Width = 445
  Height = 252
  Caption = 'MambaNet connection setup'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object EthernetGroupBox: TGroupBox
    Left = 24
    Top = 16
    Width = 393
    Height = 73
    Caption = 'MambaNet over Ethernet (Level 2)'
    TabOrder = 0
    object AdapterComboBox: TComboBox
      Left = 6
      Top = 16
      Width = 379
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      Sorted = True
      TabOrder = 0
    end
  end
  object OpenButton: TButton
    Left = 336
    Top = 178
    Width = 73
    Height = 25
    Caption = 'Open'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object TCPIPGroupBox: TGroupBox
    Left = 25
    Top = 96
    Width = 392
    Height = 73
    Caption = 'MambaNet over IP/UDP (level 3)'
    Enabled = False
    TabOrder = 2
    object URLEdit: TEdit
      Left = 8
      Top = 16
      Width = 377
      Height = 21
      TabOrder = 0
      Text = '--- URL ---'
    end
  end
  object EthernetRadioButton: TRadioButton
    Left = 16
    Top = 16
    Width = 17
    Height = 17
    Checked = True
    TabOrder = 3
    TabStop = True
    OnClick = EthernetRadioButtonClick
  end
  object TCPIPRadioButton: TRadioButton
    Left = 16
    Top = 96
    Width = 17
    Height = 17
    TabOrder = 4
    OnClick = TCPIPRadioButtonClick
  end
  object CSpinEdit1: TCSpinEdit
    Left = 272
    Top = 176
    Width = 57
    Height = 34
    TabStop = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    MaxValue = 65000
    MinValue = 1
    ParentColor = False
    ParentFont = False
    TabOrder = 5
    Value = 1
  end
  object AboutButton: TButton
    Left = 28
    Top = 178
    Width = 75
    Height = 25
    Caption = 'About ...'
    TabOrder = 6
    OnClick = AboutButtonClick
  end
end
