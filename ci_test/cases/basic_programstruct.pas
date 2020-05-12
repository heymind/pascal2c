program basic_program_struct(input,output);
const
  Pi = 3.14;
var
  r : Real;
function area(r: Real): Real;
begin
  area := r * r * Pi
end;
procedure print(area: Real);
begin
  write(area)
end;
begin
  read(r);
  print(area(r))
end.