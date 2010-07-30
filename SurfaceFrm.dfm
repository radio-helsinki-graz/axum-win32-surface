object SurfaceForm: TSurfaceForm
  Left = 478
  Top = 61
  Width = 800
  Height = 600
  HorzScrollBar.Tracking = True
  VertScrollBar.ButtonSize = 10
  VertScrollBar.Tracking = True
  Caption = 'Surface'
  Color = clBtnFace
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = SurfaceMainMenu
  OldCreateOrder = False
  Position = poMainFormCenter
  Visible = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object SurfaceMainMenu: TMainMenu
    Left = 64
    Top = 56
    object File1: TMenuItem
      Caption = 'File'
      object ConnecttoAXUM1: TMenuItem
        Caption = 'Connect to AXUM'
        OnClick = ConnecttoAXUM1Click
      end
    end
    object Window1: TMenuItem
      Caption = 'Window'
      object Reorder1: TMenuItem
        Caption = 'Re-order'
        OnClick = Reorder1Click
      end
      object AlwaysOnTopMenuItem: TMenuItem
        Caption = 'Always on top'
        OnClick = AlwaysOnTopMenuItemClick
      end
    end
  end
end
