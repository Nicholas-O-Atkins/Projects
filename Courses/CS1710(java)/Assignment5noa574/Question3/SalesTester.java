/**
 * Program -- SalesTrasaction 
 * Description - An object created to monitor the number of tables purchased and their costs
 * Author -- noa574
 */
import java.util.ArrayList;
import java.util.Scanner;
class SalesTransaction{
    private String transactionID;
    private String type;
    private double costPerTable;
    int totalTablesSold = 0;
    private int tables;
    private double untaxed;
    private double discount;
    private double totalCost;
    public SalesTransaction(String tansID, String size, double tableCost, int tablesToSell){
        transactionID = tansID;
        type = size;
        costPerTable = tableCost;
        tables = tablesToSell;
        discount = 0.0;
        totalTablesSold+=tables;
        untaxed = costPerTable * tables;
        if(tables > 20)
            discount = 0.10;
        totalCost = (untaxed-discount*untaxed)*1.15;
    }
    public int getTables(){
        return tables;
    }
    public String getSize(){
        return type;
    }
    public String getID(){
        return transactionID;
    }
    public double getTotalCost(){
        return totalCost;
    }
    public double getDiscount(){
        return discount;
    }
    public double getCostPerTable(){
        return costPerTable;
    }
    public int getTotalTables(){
        return totalTablesSold;
    }
    public void setCostPerTable(double cost){
        costPerTable = cost;
        totalCost = ((tables * costPerTable) - discount * (tables * costPerTable)) * 1.15;
    }
    public void changeTableNum(int nTables){
        int temp = tables;
        tables = nTables;
        if(tables > 20)
            discount = 0.10;
        totalTablesSold+=(tables - temp);
        totalCost = ((tables * costPerTable) - discount * (tables * costPerTable)) * 1.15; 
    }
}
/*
 * this class calls most of the programs features and tests them out, 
 * this will run until "Q" is inputted in when it asks you if you need
 * to input another transaction
 *
 * author -- noa574 (Nicholas Atkins)
 */
public class SalesTester{
    public static void main(String[] args){
        String isNewTrans = "";
        int count = 0;
        String transID = "";
        String type = "";
        double cost = 0.0;
        double newCost = 0.0;
        int newAmount = 0;
        int amountToBuy = 0;
        String yayOrNay = "";
        int totalTablesSold = 0;
        Scanner in = new Scanner(System.in);
        ArrayList<SalesTransaction> transaction = new ArrayList<SalesTransaction>();
        while(!isNewTrans.equals("Q")){
            cost = 0;
            newCost = 0;
            amountToBuy = 0;
            System.out.println("Transaction ID please: ");
            transID = in.nextLine();
            System.out.println("Size of table please: ");
            type = in.nextLine();
            System.out.println("Cost per table please: ");
            cost = in.nextDouble();
            System.out.println("How many of that table?: ");
            amountToBuy = in.nextInt();
            SalesTransaction trans1 = new SalesTransaction(transID, type, cost, amountToBuy);
            transaction.add(trans1);
            System.out.println("Transaction ID: " + transaction.get(count).getID());
            System.out.println("Size: " + transaction.get(count).getSize());
            System.out.printf("Cost Per Table: $%.2f\n",transaction.get(count).getCostPerTable());
            System.out.println("Num Tables: " + transaction.get(count).getTables());
            System.out.println("Total Tables So Far: " + (totalTablesSold + transaction.get(count).getTables()));
            System.out.printf("Discount: $%.2f\n",transaction.get(count).getDiscount());
            System.out.printf("Total Cost: $%.2f\n",transaction.get(count).getTotalCost());
            System.out.println("Would you like to change the cost per table?(Y if so)");
            yayOrNay = in.next();
            if(yayOrNay.equals("Y")){
                System.out.println("What is the new price per table?");
                newCost = in.nextDouble();
                transaction.get(count).setCostPerTable(newCost);
                System.out.printf("New Cost Per Table: $%.2f\n", transaction.get(count).getCostPerTable());
                System.out.printf("Discount: $%.2f\n",(transaction.get(count).getDiscount()*transaction.get(count).getCostPerTable()*transaction.get(count).getTables()));
                System.out.printf("Total Cost: $%.2f\n",transaction.get(count).getTotalCost());
            }
            System.out.println("Would you like to change the number of tables?(Y if so)");
            yayOrNay = in.next();
            if(yayOrNay.equals("Y")){
                System.out.println("What is the new number of tables?");
                newAmount = in.nextInt();
                int temp = transaction.get(count).getTables();
                transaction.get(count).changeTableNum(newAmount);
                System.out.println("New Num Tables: " + transaction.get(count).getTables());
                System.out.println("Total Tables So Far: " + (totalTablesSold + temp + (transaction.get(count).getTables()-temp)));
            }
            System.out.printf("Discount: $%.2f\n",(transaction.get(count).getDiscount()*transaction.get(count).getCostPerTable()*transaction.get(count).getTables()));
            System.out.printf("Total Cost: $%.2f\n",transaction.get(count).getTotalCost());
            System.out.printf("Do you wish to purchasse something else?(if not Q to quit)\n");
            isNewTrans = in.next();
            String blank = in.nextLine();
            System.out.println("");
            totalTablesSold += transaction.get(count).getTables();
            count++;
        }
    }
}