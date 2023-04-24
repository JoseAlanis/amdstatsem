# Authors: Jose C. Garcia Alanis <alanis.jcg@gmail.com>
#
# License: BSD-3-Clause
import numpy as np

import matplotlib.pyplot as plt
from matplotlib.patches import ConnectionPatch

import scipy.stats as stats
import math
# %%
# annotation function
def annotate_axes(ax, text, fontsize=20, pos=None):
    if pos is None:
        pos = [0.5, 0.5]
    ax.text(pos[0], pos[1], text, transform=ax.transAxes,
            ha="center", va="center", fontsize=fontsize, color="k")

# %%
# simulate data
mu = 0
variance = 1
sigma = math.sqrt(variance)
x = np.linspace(mu - 3*sigma, mu + 3*sigma, 100)

# intialise figure
gs_kw = dict(width_ratios=[1, 1, 1, 1, 0.2],
             height_ratios=[1, 1, 0.5, 0.5, 0.3],
             wspace=0.10, hspace=0.25)

figsize=(11, 5)
fig, axd = plt.subplot_mosaic([['X', 'lev-2', 'lev-2', 'X', 'lab_l2'],
                               ['X', 'lev-2', 'lev-2', 'X', 'lab_l2'],
                               ['lev-1a', 'lev-1b', 'lev-1c', 'lev-1d', 'lab_l1'],
                               ['lev-1a', 'lev-1b', 'lev-1c', 'lev-1d', 'lab_l1'],
                               ['lev-1ae', 'lev-1be', 'lev-1ce', 'lev-1de', 'lab_l0']],
                              gridspec_kw=gs_kw,
                              empty_sentinel="X",
                              figsize=figsize,
                              layout='constrained')

# make figure
for ax in ['lev-2', 'lev-1a', 'lev-1b', 'lev-1c', 'lev-1d']:
    x = np.linspace(mu - 3*sigma, mu + 3*sigma, 100)
    axd[ax].plot(x, stats.norm.pdf(x, mu, sigma), color='k')

offsets = [-3, -1.0, 1.0, 3]
i = 0
for ax in axd:
    if ax in {'lev-1a', 'lev-1b', 'lev-1c', 'lev-1d'}:
        con = ConnectionPatch(xyA=(0.0, 0.43),
                              coordsA=axd[ax].transData,
                              xyB=(offsets[i], -0.020),
                              coordsB=axd['lev-2'].transData,
                              color='k',
                              lw=2.0,
                              arrowstyle='->',
                              connectionstyle="Arc, rad=0.0")
        fig.add_artist(con)
        i += 1

xs = [0, 0.25, 0.50, 0.75, 1.0]
for ax in axd:
    if ax in {'lev-1ae', 'lev-1be', 'lev-1ce', 'lev-1de'}:
        axd[ax].scatter(x=xs,
                        y=[0.5, 0.5, 0.5, 0.5, 0.5],
                        s=[150, 150, 150, 150, 150],
                        color='k')
        for x in xs:
            con = ConnectionPatch(xyA=(x, 0.52),
                                  coordsA=axd[ax].transData,
                                  xyB=(x, 0.56),
                                  coordsB=axd[ax].transData,
                                  color='k',
                                  lw=2.0,
                                  arrowstyle='->',
                                  connectionstyle="Arc, rad=0.0")
            fig.add_artist(con)

for ax in ['lev-2', 'lev-1a', 'lev-1b', 'lev-1c', 'lev-1d',
           'lev-1ae', 'lev-1be', 'lev-1ce', 'lev-1de',
           'lab_l2', 'lab_l1', 'lab_l0']:
    axd[ax].spines['top'].set_visible(False)
    axd[ax].spines['right'].set_visible(False)
    axd[ax].spines['bottom'].set_visible(False)
    axd[ax].spines['left'].set_visible(False)
    axd[ax].tick_params(
        axis='both', which='both',
        bottom=False, left=False,
        labelbottom=False, labelleft=False)

for k in axd:
    params = {'text.usetex': True,
              'font.family': 'serif',
              'mathtext.fontset': 'cm'}
    plt.rcParams.update(params)
    if k == 'lev-2':
        annotate_axes(axd[k], r"$\gamma_{00}$",
                      fontsize=40)
    if k in {'lev-1a', 'lev-1b', 'lev-1c', 'lev-1d'}:
        annotate_axes(axd[k], r"$i$",
                      pos=[0.5, 0.40],
                      fontsize=40)
    if k in {'lev-1ae', 'lev-1be', 'lev-1ce', 'lev-1de'}:
        for x in xs:
            annotate_axes(axd[k], r"$m$",
                          pos=[x, -0.10],
                          fontsize=20)
    if k == 'lab_l2':
        annotate_axes(axd[k], r"\textbf{Population}")
    if k in {'lab_l0', 'lab_l1'}:
        annotate_axes(axd[k], r"\textbf{Level %s}" % str(int(k[-1])+1))
    if k == 'lab_l1':
        annotate_axes(axd[k], r"$\gamma_{00}+\nu_{0i}$",
                      pos=[0.5, 0.20],
                      fontsize=25)
    if k == 'lab_l0':
        annotate_axes(axd[k], r"$y_{mi}$",
                      pos=[0.5, -0.30],
                      fontsize=20)

fig.savefig(fname='../images/mlm_fig.png', dpi=300,
            facecolor='white')
