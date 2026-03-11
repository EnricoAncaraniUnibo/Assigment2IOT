

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DRUView view;
	LogView logger;
	
	static final String STATE_PREFIX 	=  "st:";
	static final String DIST_PREFIX 	=  "dist:";
	static final String LOG_PREFIX 	=  "lo:";
	static final String ALARM_PREFIX = "alarm:";
	static final String DRONE_PREFIX = "dr:";

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
				            view.setDistance("--");
				            break;
				        case 1: // TAKING_OFF
				            view.setDroneState("TAKING OFF");
				            view.setHangarState("NORMAL");
				            break;
				        case 2: // DRONE_OUT
				            view.setDroneState("OPERATING");
				            view.setHangarState("NORMAL");
				            view.setDistance("--");
				            break;
				        case 3: // LANDING
				            view.setDroneState("LANDING");
				            view.setHangarState("NORMAL");
				            break;
				        case 4: // PRE_ALARM
				            view.setHangarState("PRE ALARM");
				            break;
				        case 5: // ALARM
				            view.setHangarState("ALARM");
				            view.setDistance("--");
				            break;
				    }
				} else if (msg.startsWith(DIST_PREFIX)){
				    String dist = msg.substring(DIST_PREFIX.length());
                    view.setDistance(dist);
				} else if (msg.startsWith(LOG_PREFIX)){
                    String log = msg.substring(LOG_PREFIX.length());
                    logger.log(log);
                }else if(msg.startsWith(ALARM_PREFIX)){
                    String alarmMsg = msg.substring(ALARM_PREFIX.length());
                        logger.log("ALARM sent to drone");
                }else if (msg.startsWith(DRONE_PREFIX)){
                    String drone = msg.substring(DRONE_PREFIX.length());

                    switch(drone){
                        case "DRONE INSIDE":
                            view.setDroneState("REST");
                            break;
                        case "DRONE OUT":
                            view.setDroneState("OPERATING");
                            break;
                    }
                }
			} catch (Exception ex){
				ex.printStackTrace();
			}
		}
	}

}
