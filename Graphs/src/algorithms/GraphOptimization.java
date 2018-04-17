package optimize;

import graph.*;

public class GraphOptimization {
	
	private static class Point {
		double x, y;
		
		public Point() {
			x = 0;
			y = 0;
		}
		
		public Point(double x, double y) {
			this.x = x;
			this.y = y;
		}
	}

	private static Point force(Node<Double, Point> p1, Node<Double, Point> p2) {
		Point force = new Point();
		
		if(p1.getNeighbors().contains(p2) || p2.getNeighbors().contains(p1)) {
			force.x = p2.getData().x - p1.getData().x;
		}
	}

}
