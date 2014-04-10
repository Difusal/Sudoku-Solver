package gui;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import javax.swing.JButton;
import java.awt.Color;

public class Window {
	private JFrame frame;
	private BoardPanel boardPanel;

	public Window() {
		frame = new JFrame();
		
		frame.setTitle("Sudoku Solver");		
		frame.setSize(new Dimension(500, 500));
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		setPanels();
	}

	public void start() {
		frame.setVisible(true);
	}

	private void setPanels() {
		boardPanel = new BoardPanel();
		frame.getContentPane().add(boardPanel, BorderLayout.CENTER);

		JPanel solveBtnPanel = new JPanel();
		solveBtnPanel.setBackground(new Color(104, 205, 63));
		frame.getContentPane().add(solveBtnPanel, BorderLayout.SOUTH);

		JButton solveButton = new JButton("Solve");
		solveBtnPanel.add(solveButton);
	}
}
