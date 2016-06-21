
// -*- mode: c++ -*-

/***************************************************************************************************
**
** $QTCARTO_BEGIN_LICENSE:GPL3$
**
** Copyright (C) 2016 Fabrice Salvaire
** Contact: http://www.fabrice-salvaire.fr
**
** This file is part of the QtCarto library.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QTCARTO_END_LICENSE$
**
***************************************************************************************************/

/**************************************************************************************************/

#ifndef __PATH_NODE_H__
#define __PATH_NODE_H__

/**************************************************************************************************/

#include "geometry/path.h"
#include "map/viewport.h"

#include <QSGOpacityNode>

/**************************************************************************************************/

// QC_BEGIN_NAMESPACE

/**************************************************************************************************/

class QcPathNode : public QSGOpacityNode
{
public:
  QcPathNode(const QcViewport * viewport);

  void update(const QcPathDouble & path);

private:
  const QcViewport * m_viewport; // Fixme: &
  QSGGeometryNode * m_path_geometry_node;
  QSGGeometryNode * m_point_geometry_node;
};

/**************************************************************************************************/

// QC_END_NAMESPACE

/**************************************************************************************************/

#endif /* __PATH_NODE_H__ */

/***************************************************************************************************
 *
 * End
 *
 **************************************************************************************************/
