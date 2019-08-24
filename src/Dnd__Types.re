module Point = {
  type t = {
    x: float,
    y: float,
  };
};

module Delta = {
  type t = {
    x: float,
    y: float,
  };
};

module Distance = {
  type t = {
    x: float,
    y: float,
  };
};

module Dimensions = {
  type t = {
    width: float,
    height: float,
  };
};

module Rect = {
  type t = {
    top: float,
    bottom: float,
    left: float,
    right: float,
  };
};

module Margins = {
  type t = {
    top: float,
    bottom: float,
    left: float,
    right: float,
  };
};

module Borders = {
  type t = {
    top: float,
    bottom: float,
    left: float,
    right: float,
  };
};

module Paddings = {
  type t = {
    top: float,
    bottom: float,
    left: float,
    right: float,
  };
};

module Offset = {
  type t = {
    top: float,
    bottom: float,
    left: float,
    right: float,
  };
};

module Scroll = {
  type t = {
    initial: Point.t,
    current: Point.t,
    delta: Delta.t,
    max: Distance.t,
  };
};

module CanScroll = {
  type t = {
    x: bool,
    y: bool,
  };
};

module Axis = {
  type t =
    | X
    | Y;
};

module Direction = {
  type t =
    | Alpha
    | Omega;
};

module AxisDirection = {
  type t = {
    x: Direction.t,
    y: Direction.t,
  };
};

module RelativityBag = {
  type t('a) = {
    page: 'a,
    viewport: 'a,
  };
};

module Geometry = {
  type t = {
    rect: RelativityBag.t(Rect.t),
    dimensions: Dimensions.t,
    margins: Margins.t,
    borders: Borders.t,
    paddings: Paddings.t,
  };
};

module Shift = {
  type t = option(Direction.t);
};

module ScrollableElement = {
  type t = {
    element: Dom.htmlElement,
    geometry: Geometry.t,
    scroll: Scroll.t,
  };
};

module DraggableBag = {
  type t('draggableId, 'droppableId) = {
    id: 'draggableId,
    droppableId: 'droppableId,
    geometry: option(Geometry.t),
    shift: Shift.t,
    animating: bool,
    getGeometry: unit => Geometry.t,
  };

  type registrationPayload('draggableId, 'droppableId) = {
    id: 'draggableId,
    droppableId: 'droppableId,
    getGeometry: unit => Geometry.t,
  };
};

module DroppableBag = {
  type t('draggableId, 'droppableId) = {
    id: 'droppableId,
    axis: Axis.t,
    geometry: option(Geometry.t),
    scrollable: option(ScrollableElement.t),
    accept: option('draggableId => bool),
    getGeometryAndScrollable:
      unit => (Geometry.t, option(ScrollableElement.t)),
  };

  type registrationPayload('draggableId, 'droppableId) = {
    id: 'droppableId,
    axis: Axis.t,
    accept: option('draggableId => bool),
    getGeometryAndScrollable:
      unit => (Geometry.t, option(ScrollableElement.t)),
  };
};

module Ghost = {
  type t('draggableId, 'droppableId) = {
    element: Dom.htmlElement,
    draggableId: 'draggableId,
    originalDroppable: 'droppableId,
    targetDroppable: option('droppableId),
    targetingOriginalDroppable: bool,
    axis: Axis.t,
    direction: option(Direction.t),
    dimensions: Dimensions.t,
    margins: Margins.t,
    borders: Borders.t,
    delta: Delta.t,
    departurePoint: RelativityBag.t(Point.t),
    currentPoint: RelativityBag.t(Point.t),
    departureRect: RelativityBag.t(Rect.t),
    currentRect: RelativityBag.t(Rect.t),
  };
};

module Subscriptions = {
  type t = {
    install: unit => unit,
    drop: unit => unit,
  };
};

module Status = {
  type t('draggableId, 'droppableId) =
    | StandBy
    | Dragging(Ghost.t('draggableId, 'droppableId), Subscriptions.t)
    | Dropping(Ghost.t('draggableId, 'droppableId));
};

module Context = {
  type t('draggableId, 'droppableId) = {
    status: Status.t('draggableId, 'droppableId),
    target: option('droppableId),
    scroll: option(Scroll.t),
    registerDraggable:
      DraggableBag.registrationPayload('draggableId, 'droppableId) => unit,
    registerDroppable:
      DroppableBag.registrationPayload('draggableId, 'droppableId) => unit,
    disposeDraggable: 'draggableId => unit,
    disposeDroppable: 'droppableId => unit,
    getDraggableShift: 'draggableId => Shift.t,
    initDrag:
      (
        ~draggableId: 'draggableId,
        ~droppableId: 'droppableId,
        ~start: RelativityBag.t(Point.t),
        ~current: RelativityBag.t(Point.t),
        ~element: Dom.htmlElement,
        ~subscriptions: Subscriptions.t
      ) =>
      unit,
    updateGhostPosition: RelativityBag.t(Point.t) => unit,
    drop: unit => unit,
    cancelDrag: unit => unit,
  };
};

module Payload = {
  type t('draggableId, 'droppableId) = {
    context: Context.t('draggableId, 'droppableId),
  };
};

module DropResult = {
  type droppables('droppableId) = {
    prev: 'droppableId,
    next: 'droppableId,
  };

  type t('draggableId, 'droppableId) =
    | SameTarget('draggableId, 'droppableId, array('draggableId))
    | NewTarget('draggableId, droppables('droppableId), array('draggableId))
    | NoChanges;

  type trait =
    | Ghost
    | Item(Shift.t);

  type draggableIntermediateResult('draggableId) = {
    id: 'draggableId,
    trait,
    rect: RelativityBag.t(Rect.t),
    margins: Margins.t,
  };
};
