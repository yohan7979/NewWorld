$xlXMLSpreadsheet = 46

$Excel = New-Object -Com Excel.Application

$WorkBook = $Excel.Workbooks.Open("D:\dev\NewWorld\Table\ItemList.xlsx")

$WorkBook.SaveAs("D:\dev\NewWorld\Table\ItemList.xml", $xlXMLSpreadsheet)

$Excel.Quit()