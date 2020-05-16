program basic_program_struct(input,output);
const
  Pi = 3.14;
  NUMBER_3 = 3;
  LE = 'l';
var
  r : Integer;
  a : Integer;
function area(r: Integer): Integer;
begin
  a := r * r * NUMBER_3
end;
procedure print(area: Integer);
begin
  write(area)
end;
begin
  read(r);
  area(r);
  print(a)
end.