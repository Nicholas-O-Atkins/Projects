/**
 * program -- PracticeQuestions
 *
 * description -- Loading questions and answers from a file, the program will ask the user questions about the different sunjects
 *
 * author -- noa574 (Nicholas Atkins)
 */
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

public class PracticeQuestions{
    public static void main(String[] args) throws FileNotFoundException {
        Scanner in = new Scanner(System.in);
        final int ROWS = 142;
        final int COL = 2;
        String[][] qna = readInput(ROWS,COL);
        String isAGo = "";
        processMenu(qna,in);
    }

    public static String[][] readInput (int rows, int col) throws FileNotFoundException {
        File inFile = new File("questions.txt");
        Scanner forArray = new Scanner(inFile);
        forArray.useDelimiter(">>|\n");
        int i = 0;
        String temp1 = "";
        String temp2 = "";
        String[][] questionsNAnswers = new String[rows][col];
        while(forArray.hasNext()){
            temp1 = forArray.next();
            temp2 = forArray.next();
            questionsNAnswers[i][0] = temp1;
            questionsNAnswers[i][1] = temp2;
            i++;
        }
        return questionsNAnswers;
    }

    public static void processMenu(String[][] questions, Scanner in){
        int option = 0;
        String subName = "";
        System.out.println("Which option would you like questions about?");
        System.out.println("1. The World");
        System.out.println("2. Canada");
        System.out.println("3. One of the Atlantic Provinces");
        System.out.println("4. Quit");
        System.out.println();
        while(option != 1 | option != 2 | option != 3){
            subName = "";
            System.out.print("Please enter the option number(1-4): ");
            option = in.nextInt();
            System.out.println(option);
            if(option == 4)
                return;
            if(option != 4){
                switch(option){
                    case 1: 
                            subName = "The World";
                            processQuestions(questions, subName, in);
                            break;
                    case 2: 
                            subName = "Canada";
                            processQuestions(questions, subName, in);
                            break;
                    case 3: 
                            subName = "Atlantic";
                            processQuestions(questions, subName, in);
                            break;
                    default:
                            System.out.println("Pick again...");
                            break;
                }
            }
        }
    }

    public static void processQuestions(String[][] questions, String subject, Scanner in){
        int startPoint = 0;
        int endPoint = 0;
        int totQs = 0;
        int rightQs = 0;
        String answer = "";
        if(subject.equals("The World")){
            startPoint = 0;
            endPoint = 21;
        }else if(subject.equals("Canada")){
            startPoint = 21;
            endPoint = 47;
        }else{
            subject = selectRandomProvince();
            if(subject.equals("Nova Scotia")){
                startPoint = 47;
                endPoint = 68;
            }else if(subject.equals("New Brunswick")){
                startPoint = 68;
                endPoint = 89;
            }else if(subject.equals("Prince Edward Island")){
                startPoint = 89;
                endPoint = 110;
            }else{
                startPoint = 110;
                endPoint = 141;
            }
        }
        System.out.println("Here  are " + questions[startPoint][1] + " questions about " + questions[startPoint][0] + ":");
        for(int i = startPoint+1; i < endPoint; i++){
            System.out.println(questions[i][0]);
            System.out.print("Please enter your answer: ");
            answer = in.next();
            totQs++;
            if(answer.equals(questions[i][1])){
                System.out.println("Correct!");
                rightQs++;
            }
            else{
                System.out.println("Incorrect! The answer is " + questions[i][1]);
            }
        }
        displayPercentCorrect(rightQs, totQs);
    }

    public static void displayPercentCorrect(int right, int total){
        double percentTotal = (right*1.0/total)*100;
        System.out.printf("You have answered %.1f%% of the questions correctly.",percentTotal);
    }

    public static String selectRandomProvince(){
        double choice = Math.random()*100;
        String subject = "";
        if(choice < 25)
            subject = "Nova Scotia";
        else if(choice < 50)
            subject = "New Brunswick";
        else if(choice < 75)
            subject = "Prince Edward Island";
        else
            subject = "Newfoundland and Labrador";
        return subject;
    }
}
