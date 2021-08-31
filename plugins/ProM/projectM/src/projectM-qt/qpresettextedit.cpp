/**
 * projectM-qt -- Qt4 based projectM GUI 
 * Copyright (C)2003-2004 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#include "qpresettextedit.hpp"
#include <QFile>
#include <QMessageBox>

#include <QKeyEvent>

void QPresetTextEdit::keyReleaseEvent(QKeyEvent * e) {

	switch (e->key()) {
		case Qt::Key_S:
			if (e->modifiers() & Qt::ControlModifier) {
				emit(applyRequested());
			}
			e->accept();
			break;
		default:
			e->ignore();
	}
	
}

bool QPresetTextEdit::loadPresetText(QString url) {
	QFile qfile(url);
	if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning (0, "Preset File Error", QString(tr("There was a problem trying to open the preset \"%1\".  The file may no longer exist or you may not have permission to read the file.")).arg(url));
			return false;
	}


	QTextStream in(&qfile);
	
	QString buffer;
	QTextStream out(&buffer);
	while (!in.atEnd()) {
		QString line = in.readLine();
		out << line << "\n";
	}	

	this->setPlainText(out.readAll());

	return true;
}
