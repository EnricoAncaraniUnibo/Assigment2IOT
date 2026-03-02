

public class DRUController  {

    static final String CMD_TAKEOFF = "cmd:TAKEOFF";
    static final String CMD_LAND    = "cmd:LAND";
    
	SerialCommChannel channel;
	DRUView view;
	LogView logger;
	
	public DRUController(String port, DRUView view, LogView logger) throws Exception {
		this.view = view;
		this.logger = logger;
		
		channel = new SerialCommChannel(port,115200);		
		new MonitoringAgent(channel,view,logger).start();
		
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");		
	
	}
	
	public void sendTakeOff() {
        channel.sendMsg(CMD_TAKEOFF);
        logger.log("Sent: TAKEOFF");
    }

    public void sendLand() {
        channel.sendMsg(CMD_LAND);
        logger.log("Sent: LAND");
    }

}
