package gui;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JPanel;

import logic.Direction;

public class BoardPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	private double dim, horMargin, verMargin;
	private SelectedCell selectedCell;

	public BoardPanel() {
		setFocusable(true);
		setBackground(new Color(59, 89, 152));

		addMouseListener(new MyMouseAdapter());
		addMouseMotionListener(new MyMouseAdapter());
		addKeyListener(new MyKeyboardAdapter());

		selectedCell = new SelectedCell();

		requestFocus();
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);

		Graphics2D g2d = (Graphics2D) g;
		g2d.setColor(Color.BLACK);
		g2d.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);

		drawBoard(g);
	}

	private void drawBoard(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;

		if (getWidth() > getHeight()) {
			verMargin = (int) Math.round(0.1 * getHeight());
			dim = getHeight() - 2 * verMargin;
			horMargin = (getWidth() - dim) / 2;
		} else {
			horMargin = (int) Math.round(0.1 * getWidth());
			dim = getWidth() - 2 * horMargin;
			verMargin = (getHeight() - dim) / 2;
		}

		// background
		g2d.setColor(Color.WHITE);
		g2d.fillRect((int) Math.round(horMargin), (int) Math.round(verMargin),
				(int) Math.round(getWidth() - 2 * horMargin),
				(int) Math.round(getHeight() - 2 * verMargin));

		// selected cell
		if (selectedCell.isVisible()) {
			g2d.setColor(Color.YELLOW);

			double d = dim / 9;
			g2d.fillRect((int) Math.round(horMargin + d * selectedCell.getX()),
					(int) Math.round(verMargin + d * selectedCell.getY()),
					(int) Math.round(d), (int) Math.round(d));
		}

		// guidelines
		g2d.setColor(Color.GRAY);
		g2d.setStroke(new BasicStroke(Math.round(0.003 * dim)));
		for (int i = 0; i < 8; i++) {
			if (i == 2 || i == 5)
				continue;

			g2d.drawLine((int) Math.round(horMargin),
					(int) Math.round(verMargin + (i + 1) * dim / 9),
					(int) Math.round(horMargin + dim),
					(int) Math.round(verMargin + (i + 1) * dim / 9));

			g2d.drawLine((int) Math.round(horMargin + (i + 1) * dim / 9),
					(int) Math.round(verMargin),
					(int) Math.round(horMargin + (i + 1) * dim / 9),
					(int) Math.round(verMargin + dim));
		}

		// main guidelines
		g2d.setColor(Color.BLACK);
		g2d.setStroke(new BasicStroke(Math.round(0.005 * dim)));
		for (int i = 0; i < 2; i++) {
			g2d.drawLine((int) Math.round(horMargin),
					(int) Math.round(verMargin + (i + 1) * dim / 3),
					(int) Math.round(horMargin + dim),
					(int) Math.round(verMargin + (i + 1) * dim / 3));

			g2d.drawLine((int) Math.round(horMargin + (i + 1) * dim / 3),
					(int) Math.round(verMargin),
					(int) Math.round(horMargin + (i + 1) * dim / 3),
					(int) Math.round(verMargin + dim));
		}

		// outline
		g2d.setColor(Color.BLACK);
		g2d.setStroke(new BasicStroke(Math.round(0.005 * dim)));
		g2d.drawRect((int) Math.round(horMargin), (int) Math.round(verMargin),
				(int) Math.round(getWidth() - 2 * horMargin),
				(int) Math.round(getHeight() - 2 * verMargin));
	}

	private class MyMouseAdapter extends MouseAdapter {
		public void mouseClicked(MouseEvent e) {
			if (e.getX() < horMargin || e.getX() > horMargin + dim
					|| e.getY() < verMargin || e.getY() > verMargin + dim)
				selectedCell.hide();
			else {
				int x = (int) ((e.getX() - horMargin) / (dim / 9));
				int y = (int) ((e.getY() - verMargin) / (dim / 9));
				selectedCell.setLocation(new Point(x, y));
			}

			repaint();
		}

		public void mouseDragged(MouseEvent e) {
			if (e.getX() < horMargin || e.getX() > horMargin + dim
					|| e.getY() < verMargin || e.getY() > verMargin + dim)
				selectedCell.hide();
			else {
				int x = (int) ((e.getX() - horMargin) / (dim / 9));
				int y = (int) ((e.getY() - verMargin) / (dim / 9));
				selectedCell.setLocation(new Point(x, y));
			}

			repaint();
		}
	}

	private class MyKeyboardAdapter extends KeyAdapter {
		public void keyPressed(KeyEvent e) {
			switch (e.getKeyCode()) {
			case KeyEvent.VK_UP:
				selectedCell.move(Direction.UP);
				break;
			case KeyEvent.VK_LEFT:
				selectedCell.move(Direction.LEFT);
				break;
			case KeyEvent.VK_DOWN:
				selectedCell.move(Direction.DOWN);
				break;
			case KeyEvent.VK_RIGHT:
				selectedCell.move(Direction.RIGHT);
				break;
			case KeyEvent.VK_ESCAPE:
				selectedCell.hide();
				break;
			}

			repaint();
		}
	}
}
