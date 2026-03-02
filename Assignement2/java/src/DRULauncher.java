

/**
 * 
 * An example of Dashboard for the Sweeping Case Study
 * 
 * Partially implemented.
 * 
 */
class DRULauncher   {

	static DRUView view = null;
	static LogView log = null;

	public static void main(String[] args) throws Exception {	

		view = new DRUView();
		log = new LogView();
		
		String portName = "COM3";
		DRUController contr = new DRUController(portName,view,log);
		view.registerController(contr);
		
		view.display();
		log.display();
	}
}