package gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class Window {
	private JFrame frame;
	private BoardPanel boardPanel;

	public Window() {
		frame = new JFrame();

		frame.setTitle("Sudoku Solver");
		frame.setSize(new Dimension(500, 500));
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		Dimension dim = new Dimension(Toolkit.getDefaultToolkit()
				.getScreenSize());
		frame.setLocation(
				(int) Math.round(dim.getWidth() / 2 - frame.getWidth() / 2),
				(int) Math.round(dim.getHeight() / 2 - frame.getHeight() / 2));

		setPanels();
	}

	public void start() {
		frame.setVisible(true);
	}

	private void setPanels() {
		boardPanel = new BoardPanel();
		frame.getContentPane().add(boardPanel, BorderLayout.CENTER);

		JPanel solveBtnPanel = new JPanel();
		solveBtnPanel.setFocusable(false);
		solveBtnPanel.setBackground(new Color(104, 205, 63));
		frame.getContentPane().add(solveBtnPanel, BorderLayout.SOUTH);

		JButton solveButton = new JButton("Solve");
		solveButton.setFocusable(false);
		solveBtnPanel.add(solveButton);
	}
}
