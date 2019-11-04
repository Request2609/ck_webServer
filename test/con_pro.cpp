#include <iostream>
#include <thread>

class Resrouce
{
    private String name;
    private int count=1;
    private boolean flag=false;
    public synchronized void set(String name)
    {  while(flag)
        try{this.wait();}
        catch(InterruptedException e)
        {}
        this.name=name+count;
        count++;
        System.out.println(Thread.currentThread().getName()+"..生产者."+this.name);
        flag=true;
        notifyAll();
    }
    public synchronized void out()
    {    while(!flag)
        try{this.wait();}
        catch(InterruptedException e)
        {}
        System.out.println(Thread.currentThread().getName()+"..消费者............."+this.name);
        flag=false;
        notifyAll();
    }
}
class Producer implements Runnable
{
    private Resrouce r;
    Producer(Resrouce r)
    {
        this.r=r;
    }
    public void run()
    {
        while(true)
        {
            r.set("烤鸭");
        }
    }
}
class Consumer implements Runnable
{
    private Resrouce r;
    Consumer(Resrouce r)
    {
        this.r=r;
    }
    public void run()
    {
        while(true)
        {
            r.out();
        }
    }
}

public class Demo  
{

    public static void main(String[] args) 
    {       
        Resrouce r=new Resrouce();
        Producer pro=new Producer(r);
        Consumer con=new Consumer(r);
        Thread t1=new Thread(pro);
        Thread t2=new Thread(con);
        Thread t3=new Thread(pro);
        Thread t4=new Thread(con);
        t1.start();
        t2.start();
        t3.start();
        t4.start();
    }

}
