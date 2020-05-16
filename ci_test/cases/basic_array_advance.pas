program basic_array(input,output);
TYPE
  level = array[1..10, -2..3] of integer;
  END;
VAR
   test_array: level;
begin
read(test_array[1, -1]);
write(test_array[1, -1])
end.