package com.takaaki.urcap.getik2.impl;

import com.ur.urcap.api.contribution.DaemonContribution;
import com.ur.urcap.api.contribution.DaemonService;

import java.net.MalformedURLException;
import java.net.URL;

public class GetIK2DaemonService implements DaemonService {

	private DaemonContribution daemonContribution;

	public GetIK2DaemonService() {
	}

	@Override
	public void init(DaemonContribution daemonContribution) {
		this.daemonContribution = daemonContribution;
		try {
			daemonContribution.installResource(new URL("file:com/takaaki/urcap/getik2/impl/daemon/"));
		} catch (MalformedURLException e) {
		}
	}

	@Override
	public URL getExecutable() {
		try {
			return new URL("file:com/takaaki/urcap/getik2/impl/daemon/GetIK");

		} catch (MalformedURLException e) {
			return null;
		}
	}

	public DaemonContribution getDaemon() {
		return daemonContribution;
	}

}
