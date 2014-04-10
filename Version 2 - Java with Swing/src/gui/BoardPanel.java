package gui;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

public class BoardPanel extends JPanel {
	private static final long serialVersionUID = 1L;

	public BoardPanel() {
		setBackground(new Color(59, 89, 152));
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

		int dim, horMargin, verMargin;

		if (getWidth() > getHeight()) {
			verMargin = (int) (0.1 * getHeight());
			dim = getHeight() - 2 * verMargin;
			horMargin = (getWidth() - dim) / 2;
		} else {
			horMargin = (int) (0.1 * getWidth());
			dim = getWidth() - 2 * horMargin;
			verMargin = (getHeight() - dim) / 2;
		}

		// background
		g2d.setColor(Color.WHITE);
		g2d.fillRect(horMargin, verMargin, getWidth() - 2 * horMargin,
				getHeight() - 2 * verMargin);

		// guidelines
		g2d.setColor(Color.GRAY);
		g2d.setStroke(new BasicStroke((float) (0.003 * dim)));
		for (int i = 0; i < 8; i++) {
			if (i == 2 || i == 5)
				continue;

			g2d.drawLine(horMargin, verMargin + (i + 1) * dim / 9, horMargin
					+ dim, verMargin + (i + 1) * dim / 9);
			g2d.drawLine(horMargin + (i + 1) * dim / 9, verMargin, horMargin
					+ (i + 1) * dim / 9, verMargin + dim);
		}

		// main guidelines
		g2d.setColor(Color.BLACK);
		g2d.setStroke(new BasicStroke((float) (0.005 * dim)));
		for (int i = 0; i < 2; i++) {
			g2d.drawLine(horMargin, verMargin + (i + 1) * dim / 3, horMargin
					+ dim, verMargin + (i + 1) * dim / 3);
			g2d.drawLine(horMargin + (i + 1) * dim / 3, verMargin, horMargin
					+ (i + 1) * dim / 3, verMargin + dim);
		}

		// outline
		g2d.setColor(Color.BLACK);
		g2d.setStroke(new BasicStroke((float) (0.005 * dim)));
		g2d.drawRect(horMargin, verMargin, getWidth() - 2 * horMargin,
				getHeight() - 2 * verMargin);
	}
}
