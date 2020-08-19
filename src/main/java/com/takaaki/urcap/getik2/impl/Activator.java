package com.takaaki.urcap.getik2.impl;

import com.ur.urcap.api.contribution.InstallationNodeService;
import com.ur.urcap.api.contribution.DaemonService;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;

public class Activator implements BundleActivator {

	@Override
	public void start(BundleContext context) throws Exception {

		GetIK2DaemonService stringDaemonService = new GetIK2DaemonService();
		GetIK2InstallationNodeService statusMonitorInstallationNodeService = new GetIK2InstallationNodeService(stringDaemonService);

		context.registerService(DaemonService.class, stringDaemonService, null);
		context.registerService(InstallationNodeService.class, statusMonitorInstallationNodeService, null);

	}

	@Override
	public void stop(BundleContext bundleContext) throws Exception {

	}
}
