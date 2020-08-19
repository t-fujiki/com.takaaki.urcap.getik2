package com.takaaki.urcap.getik2.impl;

import com.ur.urcap.api.contribution.DaemonContribution;
import com.ur.urcap.api.contribution.InstallationNodeContribution;
import com.ur.urcap.api.domain.URCapAPI;
import com.ur.urcap.api.domain.data.DataModel;
import com.ur.urcap.api.domain.script.ScriptWriter;
import com.ur.urcap.api.ui.annotation.Label;
import com.ur.urcap.api.ui.component.LabelComponent;

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
			writer.appendLine(str);
		}

	}

}
