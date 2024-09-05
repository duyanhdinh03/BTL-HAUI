function varargout = button(varargin)
%BUTTON M-file for button.fig
%      BUTTON, by itself, creates a new BUTTON or raises the existing
%      singleton*.
%
%      H = BUTTON returns the handle to a new BUTTON or the handle to
%      the existing singleton*.
%
%      BUTTON('Property','Value',...) creates a new BUTTON using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to button_OpeningFcn.  This calling syntax produces a
%      warning when there is an existing singleton*.
%
%      BUTTON('CALLBACK') and BUTTON('CALLBACK',hObject,...) call the
%      local function named CALLBACK in BUTTON.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help button

% Last Modified by GUIDE v2.5 29-Oct-2021 16:44:56

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @button_OpeningFcn, ...
                   'gui_OutputFcn',  @button_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
   gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before button is made visible.
function button_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for button
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes button wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = button_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
x= 0:0.01:10;
y = sin(x) + 3*cos(2*x);
plot(x,y);
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
