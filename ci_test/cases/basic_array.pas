program basic_array(input,output);
TYPE
  level = array[1..10] of integer;
VAR
   test_array: level;
begin
test_array[2] := 3;
write(test_array[2])
end.