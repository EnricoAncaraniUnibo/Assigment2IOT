

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DRUView view;
	LogView logger;
	
	static final String STATE_PREFIX 	=  "st:";
	static final String DIST_PREFIX 	=  "dist:";
	static final String LOG_PREFIX 	=  "lo:";

	public MonitoringAgent(SerialCommChannel channel, DRUView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}
	
	public void run(){
		while (true){
			try {
				String msg = channel.receiveMsg();		
				if (msg.startsWith(STATE_PREFIX)){
				    String stateStr = msg.substring(STATE_PREFIX.length());
				    int stateCode = Integer.parseInt(stateStr);
				    
				    switch(stateCode){
				        case 0: view.setHangarState("DRONE INSIDE"); break;
				        case 1: view.setHangarState("TAKING OFF");   break;
				        case 2: view.setHangarState("DRONE OUT");    break;
				        case 3: view.setHangarState("LANDING");      break;
				        case 4: view.setHangarState("PRE ALARM");    break;
				        case 5: view.setHangarState("ALARM");        break;
				    }
				} else if (msg.startsWith(DIST_PREFIX)){
				    String dist = msg.substring(DIST_PREFIX.length());
                    view.setDistance(dist);
				} else if (msg.startsWith(LOG_PREFIX)){
                    String log = msg.substring(LOG_PREFIX.length());
                    logger.log(log);
                }
			} catch (Exception ex){
				ex.printStackTrace();
			}
		}
	}

}
