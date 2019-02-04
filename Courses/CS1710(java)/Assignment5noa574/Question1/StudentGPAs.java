/**
 * StudentGPAs -- Given a file with the names, number and 5 marks of students calculates and prints a file with the number and GPA on it
 * @author noa574
 */
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.PrintWriter;
public class StudentGPAs {
    public static void main( String[] args ) throws FileNotFoundException {
        File inFile = new File("Students.txt");
        Scanner in = new Scanner(inFile);
        PrintWriter outFile = new PrintWriter("ClassGPA.txt");

        double total = 0;
        double mark = 0;
        double average = 0;
        String stuName = "";
        String stuNum = "";
        double gpa = 0;

        while(in.hasNext()){
            total = 0;
            average = 0;
            stuName = in.next();
            stuNum = in.next();
            for(int i = 0; i < 5; i++){
                mark = in.nextDouble();
                total+=mark;
            }
            average = total/5;
            if(average >= 80.0)
                gpa = 4.0;
            else if(average >= 65.0)
                gpa = 3.0;
            else if(average >= 55.0)
                gpa = 2.0;
            else if(average >= 50.0)
                gpa = 1.0;
            else
                gpa = 0;
            outFile.printf("Student Number: %s GPA: %.1f\n",stuNum,gpa);
        }
        in.close();
        outFile.close();
    }
}
