	#if 0
	for(gnote::Note::List::const_iterator iter = notes.begin();
		iter != notes.end(); ++iter) {
		const gnote::Note::Ptr & note(*iter);
		qDebug() << QString::fromStdString(note->get_title());
		qDebug() << QString::fromStdString(note->file_path());
	}
	#endif


	//qDebug() << QString::fromStdString(notes[0]->file_path());
	//notes[0]->save();
	// open a specific note and get output into qstring
	// save note into new qstring
	// compare the two, they should be the same
	// quit somehow
