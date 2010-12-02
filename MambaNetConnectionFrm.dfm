object MambaNetConnectionForm: TMambaNetConnectionForm
  Left = 411
  Top = 154
  Width = 420
  Height = 172
  Caption = 'Connection setup'
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
  object OpenButton: TButton
    Left = 328
    Top = 106
    Width = 73
    Height = 25
    Caption = 'Open'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object TCPIPGroupBox: TGroupBox
    Left = 9
    Top = 8
    Width = 392
    Height = 81
    Caption = 'URL Axum engine'
    TabOrder = 1
    object URLEdit: TEdit
      Left = 8
      Top = 16
      Width = 377
      Height = 21
      TabOrder = 0
    end
    object UDPRadioButton: TRadioButton
      Left = 8
      Top = 48
      Width = 49
      Height = 25
      Caption = 'UDP'
      Checked = True
      TabOrder = 1
      TabStop = True
    end
    object TCPRadioButton: TRadioButton
      Left = 72
      Top = 48
      Width = 49
      Height = 25
      Caption = 'TCP'
      TabOrder = 2
    end
  end
  object AboutButton: TButton
    Left = 12
    Top = 106
    Width = 75
    Height = 25
    Caption = 'About ...'
    TabOrder = 2
    OnClick = AboutButtonClick
  end
end
