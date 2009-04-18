{
 ***************************************************************************
 *                                                                         *
 *   This source is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This code is distributed in the hope that it will be useful, but      *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   General Public License for more details.                              *
 *                                                                         *
 *   A copy of the GNU General Public License is available on the World    *
 *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also      *
 *   obtain it by writing to the Free Software Foundation,                 *
 *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.        *
 *                                                                         *
 ***************************************************************************
}
unit codeexplorer_figures_options;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms,
  IDEOptionsIntf, LazarusIDEStrConsts, CodeExplOpts, ExtCtrls;

type

  { TCodeExplorerFiguresOptionsFrame }

  TCodeExplorerFiguresOptionsFrame = class(TAbstractIDEOptionsEditor)
    FigureCategoriesCheckGroup: TCheckGroup;
  public
    function GetTitle: String; override;
    procedure Setup(ADialog: TAbstractOptionsEditorDialog); override;
    procedure ReadSettings(AOptions: TAbstractIDEOptions); override;
    procedure WriteSettings(AOptions: TAbstractIDEOptions); override;
    class function SupportedOptionsClass: TAbstractIDEOptionsClass; override;
  end;

implementation

{ TCodeExplorerFiguresOptionsFrame }

function TCodeExplorerFiguresOptionsFrame.GetTitle: String;
begin
  Result := lisCEFigures;
end;

procedure TCodeExplorerFiguresOptionsFrame.Setup(
  ADialog: TAbstractOptionsEditorDialog);
var
  c: TCEFigureCategory;
begin
  FigureCategoriesCheckGroup.Caption := lisCEShowFigures;
  for c := Low(TCEFigureCategory) to High(TCEFigureCategory) do
    FigureCategoriesCheckGroup.Items.Add(CodeExplorerLocalizedString(c));
end;

procedure TCodeExplorerFiguresOptionsFrame.ReadSettings(
  AOptions: TAbstractIDEOptions);
var
  c: TCEFigureCategory;
begin
  with AOptions as TCodeExplorerOptions do
    for c := Low(TCEFigureCategory) to High(TCEFigureCategory) do
      FigureCategoriesCheckGroup.Checked[ord(c)] := c in Figures;
end;

procedure TCodeExplorerFiguresOptionsFrame.WriteSettings(
  AOptions: TAbstractIDEOptions);
var
  NewCategories: TCEFigureCategories;
  c: TCEFigureCategory;
begin
  NewCategories := [];
  for c := Low(TCEFigureCategory) to high(TCEFigureCategory) do
    if FigureCategoriesCheckGroup.Checked[ord(c)] then
      Include(NewCategories, c);
  with AOptions as TCodeExplorerOptions do
    Figures := NewCategories;
end;

class function TCodeExplorerFiguresOptionsFrame.SupportedOptionsClass: TAbstractIDEOptionsClass;
begin
  Result := TCodeExplorerOptions;
end;

initialization
  {$I codeexplorer_figures_options.lrs}
  RegisterIDEOptionsEditor(GroupCodeExplorer, TCodeExplorerFiguresOptionsFrame, cdeOptionsFigures);

end.

