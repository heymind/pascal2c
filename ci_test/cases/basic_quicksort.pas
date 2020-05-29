program basic_quicksort(input, output);
type
  array_type = array[0..100000] of Integer;
var
  a: array_type;
  i, j, mid, n, x, temp: Integer;
procedure readarray;
var i: integer;
begin
  for i:=1 to 3 do
  begin
    read(temp);
    a[i]:=temp
  end
end;
procedure exchange(i,j: Integer);
begin
  x := a[i]; a[i] := a[j]; a[j] := x
end;
procedure quicksort(l,r: Integer);
var k,v: Integer;
begin
  if l < r then
  begin
    i:=l;
    j:=r;
    mid:=a[(l+r) / 2];
    while i<j do
    begin
      while a[i]<mid do i := i+1;
      while a[j]>mid do j := j-1;
      if i<=j then
      begin
        exchange(i,j);
        i := i+1;
        j := j-1
      end
    end;
    quicksort(l,j);
    quicksort(i,r)
  end

end;
begin
  readarray;
  quicksort(1, 3);
  for i:=1 to 3 do
  begin
    temp := a[i];
    write(temp)
  end
end.