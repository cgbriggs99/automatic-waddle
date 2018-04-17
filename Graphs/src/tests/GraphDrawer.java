package tests;

import javax.swing.JApplet;

import algorithms.GraphAlgs;

import java.awt.Graphics;
import graph.*;
import java.util.*;

public class GraphDrawer extends JApplet {

	/**
	 * 
	 */
	private static final long serialVersionUID = 5552919152370679974L;

	private Graph<Double, Double> graph;
	private static int dist = 10, diam = 5;

	public void paint(Graphics draw) {
		int rad = diam / 2;
		// this.graph = Graph.randomConnectedGraph(20, 30).randomWalk(10);
		Graph<Double, Double> g = Graph.readString(
				"[20 30]{(1 -> 2 0.15389073575390455), (1 -> 3 0.13843281286996045), (1 -> 4 0.5343226157843701), (3 -> 5 0.34607085943644467), (1 -> 6 0.860639788785674), (6 -> 7 0.12737940833746242), (4 - 8 0.5004022423582647), (3 -> 9 0.013196939944139285), (2 -> 10 0.9144921857616891), (10 -> 11 0.06241079372689873), (9 -> 12 0.1327448259501013), (11 - 13 0.25320390805435), (10 -> 14 0.6229238353597378), (2 -> 15 0.5799059025446706), (1 -> 16 0.3198613878353914), (15 - 17 0.14541871260141048), (8 -> 18 0.18096873628962928), (6 -> 19 0.9138093247387454), (14 -> 20 0.019777359403523875), (14 -> 1 0.2026984201586235), (1 - 10 0.4012421846627332), (3 -> 10 0.5925169110563248), (9 - 5 0.24768123755619675), (13 -> 20 0.18550555326674), (4 -> 9 0.8852778378322185), (18 -> 12 0.16605505715642355), (2 -> 16 0.7412015712545037), (12 -> 2 0.4042264782307813), (16 -> 20 0.5052524283320653), (5 -> 14 0.9370181360259174)}");
		this.graph = GraphAlgs.primAlgorithm(g);
		int[] xs = new int[graph.getNumNodes()], ys = new int[graph.getNumNodes()];

		int cx, cy;
		double scalex, scaley;
		cx = this.getWidth() / 2;
		cy = this.getHeight() / 2;
		scalex = 0.8 * cx;
		scaley = 0.8 * cy;

		for (int i = 0; i < xs.length; i++) {
			xs[i] = (int) (scalex * Math.cos(2 * i * Math.PI / xs.length) + cx);
			ys[i] = (int) (scaley * Math.sin(2 * i * Math.PI / ys.length) + cy);
			draw.fillRect(xs[i] - 2, ys[i] - 2, diam, diam);
		}

		for (Edge<Double, Double> e : graph.getEdgeList()) {
			int x1 = xs[e.getStart().getId()], y1 = ys[e.getStart().getId()], x2 = xs[e.getEnd().getId()],
					y2 = ys[e.getEnd().getId()];
			double alpha = Math.atan2((x2 - x1), (y2 - y1));
			draw.drawLine(x1 + rad + dist * Integer.signum(x2 - x1), y1 + rad + dist * Integer.signum(y2 - y1),
					x2 - rad - dist * Integer.signum(x2 - x1), y2 - rad - dist * Integer.signum(y2 - y1));
			if (e.isDirected()) {
				// draw.drawRect(x2 - 5 * Integer.signum(x2 - x1), y2 - 10 - 5 *
				// Integer.signum(y2 - y1), 10, 10);

				draw.drawLine(x2 - rad - dist * Integer.signum(x2 - x1), y2 - rad - dist * Integer.signum(y2 - y1),
						x2 - rad - Integer.signum(x2 - x1) * (int) (dist)
								+ (int) (dist * Math.cos(alpha + Math.PI / 4)),
						y2 - rad - Integer.signum(y2 - y1) * (int) (dist)
								- (int) (dist * Math.sin(alpha + Math.PI / 4)));
				draw.drawLine(x2 - rad - dist * Integer.signum(x2 - x1), y2 - rad - dist * Integer.signum(y2 - y1),
						x2 - rad - Integer.signum(x2 - x1) * (int) (dist)
								- (int) (dist * Math.cos(alpha - Math.PI / 4)),
						y2 - rad - Integer.signum(y2 - y1) * (int) (dist)
								+ (int) (dist * Math.sin(alpha - Math.PI / 4)));
			}
		}
	}

	// public static void main(String[] args) {
	// Scanner sc = new Scanner(System.in);
	//
	// GraphDrawer d = new GraphDrawer(sc.nextLine());
	//
	// d.paint(d.getGraphics());
	//
	// sc.close();
	// }
}
