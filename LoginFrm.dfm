object LoginForm: TLoginForm
  Left = 192
  Top = 140
  Width = 391
  Height = 158
  Caption = 'Login'
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
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 57
    Height = 19
    AutoSize = False
    Caption = 'Username:'
    Layout = tlCenter
  end
  object Label2: TLabel
    Left = 16
    Top = 56
    Width = 57
    Height = 19
    AutoSize = False
    Caption = 'Password:'
    Layout = tlCenter
  end
  object UsernameEdit: TEdit
    Left = 72
    Top = 16
    Width = 289
    Height = 21
    TabOrder = 0
  end
  object PasswordEdit: TEdit
    Left = 72
    Top = 56
    Width = 289
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
  end
  object LoginButtom: TButton
    Left = 224
    Top = 88
    Width = 65
    Height = 25
    Caption = 'Login'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 296
    Top = 88
    Width = 65
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
