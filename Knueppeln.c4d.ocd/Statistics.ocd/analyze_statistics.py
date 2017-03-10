#! /usr/bin/env python

import argparse
import matplotlib.pyplot as plt
import numpy
import operator
import re
import sqlite3
import sys
import urllib

# program arguments
parser = argparse.ArgumentParser(description='Analyzes the statistics of all stored Knueppeln rounds.')
parser.add_argument('-t', '--type', action='store', help='specific round type (Deathmatch, Defense, BombRun, Domination, Moba)')
parser.add_argument('-v', '--verbose', action='store_true', help='verbose')
args = parser.parse_args()

# get statistics database
urllib.urlretrieve("http://league.openclonk.org/statistics.sqlite", "statistics.sqlite")

# open the database
conn = sqlite3.connect('statistics.sqlite')

# get the statistics for all games
stats = conn.execute("SELECT statistics from games")

# init different statistics
champ_selection_stats = {}
champ_killdeaths_stats = {}
champ_spellusage_stats = {}
nr_rounds_analyzed = 0

# loop over all games in the database
for stat in stats:
	if stat[0] != None:
		stat = stat[0].decode()

		# round type check
		if args.type != None:
			round_type = re.search("\"Statistics_RoundType\":[a-zA-Z0-9:,\"]*", stat)
			if round_type != None:
				round_type = round_type.group(0).split(':')[1].replace("\"", "")
				if args.type != round_type:
					continue
			else:
				continue

		# nr rounds
		nr_rounds_analyzed += 1

		# check champ selection
		champ_selection = re.search("\"Statistics_ChampSelection\":\{[a-zA-Z0-9:,\"]*\}", stat)
		if champ_selection != None:
			champ_data = re.findall("\"[a-zA-Z0-9]*Man\":[0-9]*", champ_selection.group(0))
			# loop over all used champs
			for champ_type in champ_data:
				tp = champ_type.split(':')[0].replace("\"", "")
				cnt = int(champ_type.split(':')[1])
				# store result
				if champ_selection_stats.get(tp) == None:
					champ_selection_stats[tp] = cnt
				else:
					champ_selection_stats[tp] += cnt

		# champ kills/deaths
		champ_killdeaths = re.search("\"Statistics_ChampKillsDeaths\":\{[a-zA-Z0-9:,\"\[\]]*\}", stat)
		if champ_killdeaths != None:
			champ_data = re.findall("\"[a-zA-Z0-9]*Man\":[0-9,\[\]]*", champ_killdeaths.group(0))
			# loop over all used champs
			for champ_type in champ_data:	
				tp = champ_type.split(':')[0].replace("\"", "")
				cnt_kills = int(champ_type.split(':')[1].split(",")[0].replace("[", ""))
				cnt_deaths = int(champ_type.split(':')[1].split(",")[1].replace("]", ""))
				# store result
				if champ_killdeaths_stats.get(tp) == None:
					champ_killdeaths_stats[tp] = (cnt_kills, cnt_deaths)
				else:
					champ_killdeaths_stats[tp] += (cnt_kills, cnt_deaths)

		# champ spell usage
		champ_spellusage = re.search("\"Statistics_ChampSpellUsage\":\{[a-zA-Z0-9:,\"\[\]\{\}]*\}", stat)
		if champ_spellusage != None:
			champ_data = re.findall("\"[a-zA-Z0-9]*Man\":\{[a-zA-Z0-9\":,]*\}", champ_spellusage.group(0))
			# loop over all used champs
			for champ_type in champ_data:	
				tp = champ_type.split(':')[0].replace("\"", "")
				# store result
				if champ_spellusage_stats.get(tp) == None:
					champ_spellusage_stats[tp] = {}
				# loop over used spells
				champ_spells = re.findall("\"[a-zA-Z]*\":[0-9]+", champ_type)
				for spell in champ_spells:
					spell_type = spell.split(':')[0].replace("\"", "")
					spell_cnt = int(spell.split(':')[1])
					if champ_spellusage_stats[tp].get(spell_type) == None:
						champ_spellusage_stats[tp][spell_type] = spell_cnt
					else:
						champ_spellusage_stats[tp][spell_type] += spell_cnt

# construct champ stats
champ_selection_stats = sorted(champ_selection_stats.items(), key=operator.itemgetter(1))
champ_selection_stats.reverse()
champ_killdeaths_stats = sorted(champ_killdeaths_stats.items(), key=operator.itemgetter(1))
champ_killdeaths_stats.reverse()
champ_spellusage_stats = sorted(champ_spellusage_stats.items(), key=operator.itemgetter(1))
champ_spellusage_stats.reverse()

# combine champ spell usage
champ_spellusage_stats_combined = {}
for champ in champ_spellusage_stats:
	for spell in champ[1].items():
		spell_type = spell[0]
		spell_cnt = spell[1]
		if champ_spellusage_stats_combined.get(spell_type) == None:
			champ_spellusage_stats_combined[spell_type] = spell_cnt
		else:
			champ_spellusage_stats_combined[spell_type] += spell_cnt
champ_spellusage_stats_combined = sorted(champ_spellusage_stats_combined.items(), key=operator.itemgetter(1))
champ_spellusage_stats_combined.reverse()

# verbose printing
if args.verbose:
	# nr of rounds
	print "================================"
	print "number of rounds analyzed: ", nr_rounds_analyzed
	print "================================"
	# print champ selection
	print "================================"
	print "champ selection:"
	print "================================"
	for champ in champ_selection_stats:
		print champ[0], ": ", champ[1]
	print "================================"

	# print champ kills/deaths
	print "================================"
	print "champ kills/deaths:"
	print "================================"
	for champ in champ_killdeaths_stats:
		print champ[0], ": ", champ[1][0], "/", champ[1][1]
	print "================================"

	# print champ spell usage
	print "================================"
	print "champ spell usage:"
	print "================================"
	for spell in champ_spellusage_stats_combined:
		print spell[0], ": ", spell[1]
	print "================================"


colors = ['darkolivegreen','darkorange','bisque','brown','blueviolet']
# bar chart: champ selection
labels = []
sizes = []
for champ in champ_selection_stats:
	labels.append(champ[0])
	sizes.append(champ[1])
plt.rcdefaults()
fig, ax = plt.subplots()
y_pos = numpy.arange(len(labels))
ax.barh(y_pos, sizes, align='center', color=colors, edgecolor='none')
ax.set_yticks(y_pos)
ax.set_yticklabels(labels)
ax.invert_yaxis()
ax.set_xlabel('times selected')
plt.savefig('stats_champ_selection.pdf')

# bar chart: kills/deaths
labels = []
sizes = []
for champ in champ_selection_stats:
	labels.append(champ[0])
	sizes.append(champ[1])
plt.rcdefaults()
fig, ax = plt.subplots()
y_pos = numpy.arange(len(labels))
ax.barh(y_pos, sizes, align='center', color=colors, edgecolor='none')
ax.set_yticks(y_pos)
ax.set_yticklabels(labels)
ax.invert_yaxis()
ax.set_xlabel('times selected')
plt.savefig('stats_champ_killsdeaths.pdf')

# bar chart: spell usage
labels = []
sizes = []
for champ in champ_spellusage_stats_combined:
	labels.append(champ[0])
	sizes.append(champ[1])
plt.rcdefaults()
fig, ax = plt.subplots()
y_pos = numpy.arange(len(labels))
ax.barh(y_pos, sizes, align='center', color=colors, edgecolor='none')
ax.set_yticks(y_pos)
ax.set_yticklabels(labels)
ax.invert_yaxis()
ax.set_xlabel('times used')
plt.savefig('stats_champ_spellusage.pdf')

# close the database
conn.close()
