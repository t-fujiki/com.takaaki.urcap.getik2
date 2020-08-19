package com.takaaki.urcap.getik2.impl;

import com.ur.urcap.api.contribution.InstallationNodeContribution;
import com.ur.urcap.api.contribution.InstallationNodeService;
import com.ur.urcap.api.domain.URCapAPI;

import java.io.InputStream;

import com.ur.urcap.api.domain.data.DataModel;

public class GetIK2InstallationNodeService implements InstallationNodeService {

	public GetIK2InstallationNodeContribution contribution = null;
	public GetIK2DaemonService stringDaemonService;

	public GetIK2InstallationNodeService(GetIK2DaemonService stringDaemonService) {
		this.stringDaemonService = stringDaemonService;

	}

	@Override
	public InstallationNodeContribution createInstallationNode(URCapAPI api, DataModel model) {

		contribution = new GetIK2InstallationNodeContribution(api, model, stringDaemonService);

		return contribution;
	}

	@Override
	public String getTitle() {
		return "GetIK2";
	}

	@Override
	public InputStream getHTML() {

		return this.getClass().getResourceAsStream("/com/takaaki/urcap/getik2/impl/installation_en.html");

	}
}
