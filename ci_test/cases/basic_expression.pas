program basic_expression(input,output);
var
  k,i : Integer;
begin
  read(k);
  i:=10;
  if 10 + 20 * 10 >= 10 * (10 / (4 + 6)) then
    write(not 1);
  if (k > 10) and (k <= 100) then
    write(1)
  else
    write(not 1);
  write(i)
end.