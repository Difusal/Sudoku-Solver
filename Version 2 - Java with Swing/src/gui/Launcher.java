package gui;

import javax.swing.UIManager;

public class Launcher {
	public static void main(String[] args) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			e.printStackTrace();
		}

		Window window = new Window();
		window.start();
	}
}
