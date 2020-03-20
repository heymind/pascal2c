program example(input,output);
TYPE
  level = array[1..10, 1..50] of integer;
  TStudent = Record
    Age : Integer;
    StudNum : Integer;
    Score : Real;
  END;
VAR
  x,y:real;
  student:TStudent;
  BEGIN
    Student.Age := 20;
    Student.StudNum := 9507;
    Student.Score := 87;
  END;
function gcd(a,b:integer):real;
begin
if b=0 then gcd:=a
else gcd:=gcd(b, a mod b)
end;
begin
read(x, y, a, b, c);
write(gcd(x, y))
end.