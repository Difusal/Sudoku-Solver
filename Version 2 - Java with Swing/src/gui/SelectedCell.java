package gui;

import java.awt.Point;

import logic.Direction;

public class SelectedCell extends Point {
	private static final long serialVersionUID = 1L;

	public SelectedCell() {
		this.setLocation(new Point(-1, -1));
	}

	public void move(Direction direction) {
		switch (direction) {
		case UP:
			if (y > 0)
				y--;
			break;
		case LEFT:
			if (x > 0)
				x--;
			break;
		case DOWN:
			if (y < 8)
				y++;
			break;
		case RIGHT:
			if (x < 8)
				x++;
			break;
		}
	}

	public boolean isVisible() {
		return (0 <= x && x < 9 && 0 <= y && y < 9);
	}

	public void hide() {
		x = -1;
		y = -1;
	}

	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}
}
