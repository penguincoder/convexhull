import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;

public class GeneratePoints extends JFrame {
   protected Vector points;
   protected String fileName;

   public GeneratePoints(String fn) {
      super("Point Generator - use mouse!");
      points = new Vector();
      fileName = fn;      
      getContentPane().setBackground(Color.black);
      addWindowListener(new GenerateWindowListener());
      addMouseListener(new PointMouseListener());
      setSize(510, 510);
      show();
   }

   protected class GenerateWindowListener extends WindowAdapter {
      public void windowClosing(WindowEvent e) {
         try {
           PrintWriter pw = new PrintWriter(new FileWriter(fileName));
           for (int i = 0; i < points.size(); i++) {
              Point pt = (Point) points.elementAt(i);
              pw.println((int) pt.getX() + " " + (500 - (int) pt.getY()) );
           }
           pw.close();
         }
         catch (IOException ioe) {
            System.out.println("IO problem");
         }
         System.exit(0);
      }
   }

   protected class PointMouseListener extends MouseAdapter {
      public void mouseReleased(MouseEvent e) {
         points.add(new Point(e.getX(), e.getY()));
         Graphics page = GeneratePoints.this.getGraphics();
         page.setColor(Color.red);
         page.fillOval(e.getX(), e.getY(), 3, 3);
      }
   }

   public static void main(String[] args) {
      GeneratePoints gui;
      if (args.length > 0)
          gui = new GeneratePoints(args[0]);
      else
          System.out.println("Usage: GeneratePoints filename");
   }
}
