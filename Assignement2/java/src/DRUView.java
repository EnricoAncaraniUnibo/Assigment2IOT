import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class DRUView extends JFrame implements ActionListener {
    private JButton takeOffButton;
    private JButton landButton;
    private JTextField hangarStateField;
    private JTextField droneStateField;
    private JTextField distanceField;
    private DRUController controller;

    public DRUView(){
        super(".:: Drone Remote Unit ::.");
        setSize(400, 300);
        this.setResizable(false);

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
        mainPanel.add(Box.createRigidArea(new Dimension(0, 10)));

        JPanel hangarPanel = new JPanel();
        hangarPanel.setLayout(new BoxLayout(hangarPanel, BoxLayout.LINE_AXIS));
        hangarStateField = new JTextField("NORMAL");
        hangarStateField.setEditable(false);
        hangarPanel.add(new JLabel("Hangar state: "));
        hangarPanel.add(hangarStateField);
        mainPanel.add(hangarPanel);

        JPanel dronePanel = new JPanel();
        dronePanel.setLayout(new BoxLayout(dronePanel, BoxLayout.LINE_AXIS));
        droneStateField = new JTextField("REST");
        droneStateField.setEditable(false);
        dronePanel.add(new JLabel("Drone state:  "));
        dronePanel.add(droneStateField);
        mainPanel.add(dronePanel);

        JPanel distPanel = new JPanel();
        distPanel.setLayout(new BoxLayout(distPanel, BoxLayout.LINE_AXIS));
        distanceField = new JTextField("--");
        distanceField.setEditable(false);
        distPanel.add(new JLabel("Distance: "));
        distPanel.add(distanceField);
        mainPanel.add(distPanel);

        mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

        JPanel buttonPanel = new JPanel();
        takeOffButton = new JButton("TAKE OFF");
        landButton    = new JButton("LAND");
        takeOffButton.addActionListener(this);
        landButton.addActionListener(this);
        buttonPanel.add(takeOffButton);
        buttonPanel.add(landButton);
        mainPanel.add(buttonPanel);

        setContentPane(mainPanel);
        addWindowListener(new WindowAdapter(){
            public void windowClosing(WindowEvent ev){
                System.exit(-1);
            }
        });
    }

    public void display() {
        SwingUtilities.invokeLater(() -> this.setVisible(true));
    }

    public void registerController(DRUController contr){
        this.controller = contr;
    }

    public void setHangarState(String state){
        SwingUtilities.invokeLater(() -> hangarStateField.setText(state));
    }

    public void setDroneState(String state){
        SwingUtilities.invokeLater(() -> droneStateField.setText(state));
    }

    public void setDistance(String dist){
        SwingUtilities.invokeLater(() -> distanceField.setText(dist));
    }

    public void actionPerformed(ActionEvent ev){
        try {
            if (ev.getSource() == takeOffButton){
                controller.sendTakeOff();
            } else if (ev.getSource() == landButton){
                controller.sendLand();
            }
        } catch (Exception ex){
            ex.printStackTrace();
        }
    }
}