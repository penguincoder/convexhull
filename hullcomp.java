import java.io.*;
import java.lang.Runtime;

public class hullcomp	{

	public static void main ( String[] args )	{
		long start = 0, end = 0, count = 0;
		long low = 0, high = 0;
		String outputname = "";
		Runtime run = Runtime.getRuntime();
		PrintStream output = null;
		
		if ( args.length < 2 )	{
			System.out.println ( "Usage: java hullcomp <low> <high> [output]" );
			System.exit ( 1 );
		}
		else	{
			low = Long.parseLong ( args[0] );
			high = Long.parseLong ( args[1] );
		}
		if ( args.length == 3 )	{
			outputname = args[2];
		}
		else	{
			outputname = "hullcomp.out";
		}

		try	{
			output = new PrintStream ( new FileOutputStream ( outputname ) );
			System.setOut ( output);
		}
		catch ( IOException exception )	{}

		System.out.println ( "Size\tSmash\tQuick" );
		try	{
			for ( count = low; count < high; count = count * 2 )	{
				start = System.currentTimeMillis();
				Process test = run.exec ( "./hull --b --p " + count + "--o b" + count + ".out" );
				test.waitFor();
				end = System.currentTimeMillis();
				System.out.print ( count + "\t" + (end - start) );
				start = System.currentTimeMillis();
				test = run.exec ( "./hull --p " + count + "--o " + count + ".out" );
				test.waitFor();
				end = System.currentTimeMillis();
				System.out.println ( "\t" + (end - start) );
			}
			start = System.currentTimeMillis();
			Process test = run.exec ( "./hull --b --p " + high );
			test.waitFor();
			end = System.currentTimeMillis();
			System.out.print ( high + "\t" + (end - start) );
			start = System.currentTimeMillis();
			test = run.exec ( "./hull --p " + high );
			test.waitFor();
			end = System.currentTimeMillis();
			System.out.println ( "\t" + (end - start) );
		}
		catch ( Exception exception )	{
			exception.printStackTrace();
		}

		try	{
			output.flush();
			output.close();
		}
		catch ( Exception exception )	{}
		
	}
}
