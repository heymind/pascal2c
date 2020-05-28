program basic_line(input,output);
var a,b,c,d,e:integer;
procedure pro_test(x,y:integer);
  begin
    if x = 1 then a := 1
    else a := 0;
    if y = 1 then b := 1
    else b := 0
  end;
begin
    read(a,b);
    pro_test(a = 0,b > 10);
    c := 100;
    if a = 1 then c := 20
    else c := 10;
    e := 0;
    for d := 1 to 10
    do e := e + 1;
    write(a,b,c,d,e)
end.
