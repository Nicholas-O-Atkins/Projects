/**
 * FrustumContainer -- Given the hight and the 
 * two radius' of the containers it creates
 * and object and then calculates the volume of 
 * that object, it also keeps track of 
 * the count of containers
 * @author noa574
 */
import java.util.Scanner;
import java.util.ArrayList;

class FrustumContainer{
    private double height;
    private double radius1;
    private double radius2;
    private double volume;
    static double containerNumber;
    
    public FrustumContainer(double h, double r1, double r2){
        height = h;
        radius1 = r1;
        radius2 = r2;
        volume = ((Math.PI * height)/3) * (Math.pow(radius1, 2)+ Math.pow(radius2, 2) + radius1 * radius2);
        containerNumber++;
    }
    public double getHeight(){
        return height;
    }
    public double getRad1(){
        return radius1;
    }
    public double getRad2(){
        return radius2;
    }
    public void setHeight(double newH){
        height = newH;
    }
    public void setRad1(double newRad1){
        radius1 = newRad1;
    }
    public void setRad2(double newRad2){
        radius2 = newRad2;
    }
    public double getVolume(){
        return volume;
    }
    public double getNumber(){
        return containerNumber;
    }
}
public class ContainerTester{
    public static void main(String[] args){
        double height = 0;
        double radius1 = 0;
        double radius2 = 0;
        Scanner in = new Scanner(System.in);
        
        ArrayList<FrustumContainer> container = new ArrayList<FrustumContainer>();
        System.out.print("Please insert the number of containers you wish to have: ");
        int numContain = in.nextInt();
        System.out.println(numContain);
        for(int i = 0; i < numContain; i++){
            System.out.println("Please insert the height of the container: ");
            height = in.nextDouble();
            System.out.println("Please insert the bottom radius of the container: ");
            radius1 = in.nextDouble();
            System.out.println("Please insert the upper radius of the container: ");
            radius2 = in.nextDouble();
            FrustumContainer c1 = new FrustumContainer(height,radius1,radius2);
            container.add(c1);
            System.out.printf("Container Number: %.1f\n",container.get(i).getNumber());
            System.out.printf("H: %.1f\n",container.get(i).getHeight());
            System.out.printf("R1: %.1f\n",container.get(i).getRad1()); 
            System.out.printf("R2: %.1f\n",container.get(i).getRad2());        
            System.out.println("Litres: "+ container.get(i).getVolume()/1000);
            System.out.println();
        }          
    }
}