import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import com.sun.image.codec.jpeg.*;
import java.awt.image.*;

public class PlotConvexHull extends JFrame {
   protected Vector points;
   protected Polygon convexHull;
   protected JMenuBar mb;
   protected JMenu menu;
   protected JMenuItem go;
   protected String fileName;

   public PlotConvexHull(String fn) {
      super("Convex Hull Plotter - just watch!");
      fileName = fn;
      points = new Vector();
      convexHull = new Polygon();
      try {
         BufferedReader br = new BufferedReader(new FileReader(fn));
         String line = br.readLine();
		 int width = 500;
		 int height = 500;
         while (line != null && line.length() > 1) {
            StringTokenizer st = new StringTokenizer(line);
            int x = Integer.parseInt(st.nextToken());
            int y = Integer.parseInt(st.nextToken());
            points.add(new Point(x + 5, 500 - y));
            line = br.readLine();
         }
         line = br.readLine();
         while (line != null && line.length() > 1) {
            StringTokenizer st = new StringTokenizer(line);
            int x = Integer.parseInt(st.nextToken());
            int y = Integer.parseInt(st.nextToken());
            convexHull.addPoint(x + 5, 500 - y);
            line = br.readLine();
         }
         br.close();
      }
      catch (IOException e) {
         System.out.println("IO Problem");
         System.exit(0);
      }
      getContentPane().setBackground(Color.white);
      addWindowListener(new PlotWindowListener());
      setSize(510, 510);
      show();
   }

   public void paint(Graphics g) {
      super.paint(g);
      getContentPane().setBackground(Color.white);
      g.setColor(Color.red);
      for (int i = 0; i < points.size(); i++) {
         Point pt = (Point) points.elementAt(i);
         g.fillOval((int) pt.getX(), (int) pt.getY(), 3, 3);
      }
      g.setColor(Color.blue);
      g.drawPolygon(convexHull); 
   }

   protected class PlotWindowListener extends WindowAdapter {
      public void windowClosing(WindowEvent e) {
         Image img = createImage(getWidth(), getHeight());
         Graphics page = img.getGraphics();
         paint(page);
         try {
            OutputStream out = new FileOutputStream(fileName + ".jpg");
            JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(out);
            encoder.encode((BufferedImage) img);
            out.close();
         }
         catch (Exception exc) {
         }
         System.exit(0);
      }
   }

   public static void main(String[] args) {
      PlotConvexHull gui;
      if (args.length > 0)
          gui = new PlotConvexHull(args[0]);
      else
          System.out.println("Usage: PlotConvexHull filename");
   }
}
