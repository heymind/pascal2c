program basic_array_advance(input,output);
TYPE
  level = array[1..10, 1..3] of integer;
VAR
   test_array: level;
begin
read(test_array[1, 1]);
write(test_array[1, 1])
end.
