

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
				        case 0: // DRONE_INSIDE
				            view.setDroneState("REST");
				            view.setHangarState("NORMAL");
				            break;
				        case 1: // TAKING_OFF
				            view.setDroneState("TAKING OFF");
				            view.setHangarState("NORMAL");
				            break;
				        case 2: // DRONE_OUT
				            view.setDroneState("OPERATING");
				            view.setHangarState("NORMAL");
				            break;
				        case 3: // LANDING
				            view.setDroneState("LANDING");
				            view.setHangarState("NORMAL");
				            break;
				        case 4: // PRE_ALARM
				            view.setDroneState("--");
				            view.setHangarState("PRE ALARM");
				            break;
				        case 5: // ALARM
				            view.setDroneState("--");
				            view.setHangarState("ALARM");
				            break;
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
