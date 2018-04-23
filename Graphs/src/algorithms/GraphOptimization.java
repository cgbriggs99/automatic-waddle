package algorithms;

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

	private static final double coulombConst = 1, hookeConst = 1;
	
	private static Point force(Node<Double, Point> p1, Node<Double, Point> p2) {
		Point force = new Point();
		
		if(p1.getNeighbors().contains(p2) || p2.getNeighbors().contains(p1)) {
			force.x = (p2.getData().x - p1.getData().x) * hookeConst;
			force.y = (p2.getData().y - p1.getData().y) * hookeConst;
		} else {
			force.x = 0;
			force.y = 0;
		}
		double dist = Math.hypot(p2.getData().x - p1.getData().x, p2.getData().y - p1.getData().y);
		
		force.x -= coulombConst * (p2.getData().x - p1.getData().x) / (dist * dist * dist);
		force.y -= coulombConst * (p2.getData().y - p1.getData().y) / (dist * dist * dist);
		return (force);
	}

}
