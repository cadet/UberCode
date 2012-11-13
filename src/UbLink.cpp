#include <QPainter>
#include <iostream>
#include "UbLink.h"
#include "UbNode.h"
#include "UbInletNode.h"
#include "UbOutletNode.h"

namespace Uber {
	UbLink::UbLink( QGraphicsItem *parent,  QGraphicsScene *scene )
		:QGraphicsPathItem(parent, scene),
		m_IsSet( false ),
		m_StartNode( NULL ),
		m_EndNode( NULL ),
		m_StartPos(QPointF(0.f, 0.f)),
		m_EndPos(QPointF(0.f, 0.f)),
		m_IsChanging(false),
		m_IsHardLink( false )
	{

	}

	UbLink::~UbLink()
	{
		std::cout << "UbLink destructor was called" << std::endl;
	}

	void UbLink::updatePath()
	{
		QPointF startPos = isChanging() ? m_StartPos : m_StartNode->scenePos();
		QPointF endPos	 = isChanging() ? m_EndPos	 : m_EndNode->scenePos();

		QPainterPath bezierPath;
		bezierPath.moveTo( startPos );
		qreal dist   = endPos.x() - startPos.x();
		qreal offset = 0.5f*dist; 
		QPointF cpA  = startPos + QPointF( offset, 0 );
		QPointF cpB  = endPos   - QPointF( offset, 0 );
		bezierPath.cubicTo( cpA, cpB, endPos );
		QPainterPathStroker outliner;
		outliner.setWidth(4.f);
		outliner.setCapStyle( Qt::RoundCap );
		setPath( outliner.createStroke(bezierPath) );
	}

	void UbLink::setStartNode( UbNode* start )
	{
		m_StartNode = start;
	}

	void UbLink::setEndNode( UbNode* end )
	{
		m_EndNode = end;
	}

	void UbLink::setNodes( UbNode* start, UbNode* end )
	{
		setStartNode( start );
		setEndNode( end );
		updatePath();
	}

	void UbLink::setStartPos( QPointF start )
	{
		m_StartPos = start;
	}

	void UbLink::setEndPos( QPointF end )
	{
		m_EndPos   = end;
	}

	UbNode* UbLink::getStartNode() const
	{
		return m_StartNode;
	}

	UbNode*	UbLink::getEndNode() const
	{
		return m_EndNode;
	}

	void UbLink::startedChanging()
	{
		m_IsChanging = true;
	}

	void UbLink::finishedChanging( bool hardLink )
	{
		m_IsHardLink = hardLink;
		m_IsChanging = false;
		if ( m_IsHardLink )
			m_StartNode->link( m_EndNode );
		updatePath();
	}

	bool UbLink::isChanging() const
	{
		return m_IsChanging;
	}

	void UbLink::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
	{
		Q_UNUSED(option)
			Q_UNUSED(widget)
			updatePath();
		QPen pen;
		pen.setWidthF(1.f);
		pen.setBrush(Qt::black);	
		painter->setPen(pen);
		painter->setBrush(QColor(177,189,180));
		painter->drawPath(path());
	}

	void UbLink::mousePressEvent( QGraphicsSceneMouseEvent * event )
	{
		if ( event->button() == Qt::RightButton )
		{
			// There are 2 cases that need to be handled differently. i) The link can be a framework link between 2 (framework ) blocks.
			// ii) The link can be between a UbIOBlock and a framework block.
			//if ( isSoftLink() )
			//{
			//	
			//} 
			//else if ( isHardLink() )
			//{

			//}
		}
	}
	void UbLink::setHardLink( bool isHard )
	{
		m_IsHardLink = isHard;
	}
}