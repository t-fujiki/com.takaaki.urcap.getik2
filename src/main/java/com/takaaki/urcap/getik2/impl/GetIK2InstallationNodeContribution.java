package com.takaaki.urcap.getik2.impl;

import com.ur.urcap.api.contribution.DaemonContribution;
import com.ur.urcap.api.contribution.InstallationNodeContribution;
import com.ur.urcap.api.domain.URCapAPI;
import com.ur.urcap.api.domain.data.DataModel;
import com.ur.urcap.api.domain.robot.RobotModel;
import com.ur.urcap.api.domain.robot.RobotModel.RobotSeries;
import com.ur.urcap.api.domain.robot.RobotModel.RobotType;
import com.ur.urcap.api.domain.script.ScriptWriter;
import com.ur.urcap.api.ui.annotation.Input;
import com.ur.urcap.api.ui.annotation.Label;
import com.ur.urcap.api.ui.component.InputEvent;
import com.ur.urcap.api.ui.component.InputTextField;
import com.ur.urcap.api.ui.component.LabelComponent;
import com.ur.urcap.api.ui.component.InputEvent.EventType;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import java.awt.image.BufferedImage;

public class GetIK2InstallationNodeContribution implements InstallationNodeContribution {

	public URCapAPI api;
	public DataModel model;
	public GetIK2DaemonService getIK2DaemonService;

	@Label(id = "figure")
	public LabelComponent figure;
	private BufferedImage imagefigure;

	@Input(id = "inputDeltaA")
	public InputTextField inputDeltaA;

	@Input(id = "inputDeltaD")
	public InputTextField inputDeltaD;

	@Input(id = "inputDeltaAlpha")
	public InputTextField inputDeltaAlpha;

	@Input(id = "inputDeltaTheta")
	public InputTextField inputDeltaTheta;

	@Input(id = "inputDeltaA")
	private void onChange_inputDeltaA(InputEvent event) {
		if (event.getEventType() == EventType.ON_CHANGE) {
			setModelonEvent("inputDeltaA", event);
		}
	}

	@Input(id = "inputDeltaD")
	private void onChange_inputDeltaD(InputEvent event) {
		if (event.getEventType() == EventType.ON_CHANGE) {
			setModelonEvent("inputDeltaD", event);
		}
	}

	@Input(id = "inputDeltaAlpha")
	private void onChange_inputDeltaAlpha(InputEvent event) {
		if (event.getEventType() == EventType.ON_CHANGE) {
			setModelonEvent("inputDeltaAlpha", event);
		}
	}

	@Input(id = "inputDeltaTheta")
	private void onChange_inputDeltaTheta(InputEvent event) {
		if (event.getEventType() == EventType.ON_CHANGE) {
			setModelonEvent("inputDeltaTheta", event);
		}
	}

	private void setModelonEvent(String name, InputEvent event) {
		if (event.getComponent() instanceof InputTextField) {
			InputTextField text = (InputTextField) event.getComponent();

			if (!text.getText().isEmpty())
				model.set(name, text.getText());
			else
				model.remove(name);

		}
	}

	public String getURType() {
		RobotSeries series = api.getProgramAPIProvider().getSystemAPI().getRobotModel().getRobotSeries();
		RobotType type = api.getProgramAPIProvider().getSystemAPI().getRobotModel().getRobotType();

		if (series == RobotSeries.CB3) {
			if (type == RobotType.UR3)
				return "UR3";
			else if (type == RobotType.UR5)
				return "UR5";
			else if (type == RobotType.UR10)
				return "UR10";

		} else if (series == RobotSeries.E_SERIES) {
			if (type == RobotType.UR3)
				return "UR3e";
			else if (type == RobotType.UR5)
				return "UR5e";
			else if (type == RobotType.UR10)
				return "UR10e";
			else if (type == RobotType.UR16)
				return "UR16e";
		}

		return null;

	}

	/**
	 * スクリプトファイルの読み込み
	 * 
	 * @param filename ファイル名
	 * @return 文字列の配列
	 */
	public String[] readScriptFile(String filename) {
		try {

			BufferedReader br = new BufferedReader(
					new InputStreamReader(this.getClass().getResourceAsStream(filename)));

			ArrayList<String> list = new ArrayList<String>();

			String addstr;
			while ((addstr = br.readLine()) != null) {
				list.add(addstr);
			}

			br.close();
			String[] res = list.toArray(new String[0]);
			return res;

		} catch (IOException e) {
			return null;
		}

	}

	/**
	 * コストラクタ
	 * 
	 * @param api   urcapAPI
	 * @param model データモデル
	 */
	public GetIK2InstallationNodeContribution(URCapAPI api, DataModel model, GetIK2DaemonService stringDaemonService) {
		this.api = api;
		this.model = model;
		this.getIK2DaemonService = stringDaemonService;

		if (stringDaemonService.getDaemon().getState() == DaemonContribution.State.STOPPED)
			stringDaemonService.getDaemon().start();

		imagefigure = null;
		try {

			imagefigure = ImageIO.read(getClass().getResource("/com/takaaki/urcap/getik2/impl/ik.png"));

		} catch (IOException e) {
			System.out.println("Loading image error");
		}

	}

	@Override
	public void openView() {
		inputDeltaA.setText(model.get("inputDeltaA", "0,0,0,0,0,0"));
		inputDeltaD.setText(model.get("inputDeltaD", "0,0,0,0,0,0"));
		inputDeltaAlpha.setText(model.get("inputDeltaAlpha", "0,0,0,0,0,0"));
		inputDeltaTheta.setText(model.get("inputDeltaTheta", "0,0,0,0,0,0"));

		figure.setImage(imagefigure);
	}

	@Override
	public void closeView() {

	}

	public boolean isDefined() {
		return false;
	}

	@Override
	public void generateScript(ScriptWriter writer) {

		String[] scripts = readScriptFile("/com/takaaki/urcap/getik2/impl/getik2.script");

		for (String str : scripts) {
			String line = str.replace("{inputDeltaA}", model.get("inputDeltaA", "0.0, 0.0, 0.0, 0.0, 0.0, 0.0"));
			line = line.replace("{inputDeltaD}", model.get("inputDeltaD", "0.0, 0.0, 0.0, 0.0, 0.0, 0.0"));
			line = line.replace("{inputDeltaAlpha}", model.get("inputDeltaAlpha", "0.0, 0.0, 0.0, 0.0, 0.0, 0.0"));
			line = line.replace("{inputDeltaTheta}", model.get("inputDeltaTheta", "0.0, 0.0, 0.0, 0.0, 0.0, 0.0"));
			line = line.replace("{UR}", "\"" + getURType() + "\"");
			writer.appendLine(line);
		}

	}

}
