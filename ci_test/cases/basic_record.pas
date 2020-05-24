program basic_record(input,output);
TYPE
    TStudent = Record
        Name : Char;
        Age : Integer;
        StudNum : Integer;
        Score : Real;
    END;
VAR
    Student1 : TStudent;
    age : Integer;
BEGIN
    read(age);
    Student1.Age := age;
    Student1.StudNum := 950007;
    Student1.Score := 87.5;
    write(Student1.Age)
END.