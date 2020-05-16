program basic_process(input,output);
var a,b:integer;
procedure swap(var x,y:integer);
  var t:integer;
  begin
    t:=x;x:=y;y:=t
  end;
begin
  read(a,b);
  swap(a,b);
  write(a,b)
end.
