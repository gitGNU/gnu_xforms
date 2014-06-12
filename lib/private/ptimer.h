/*
 *  This file is part of the XForms library package.
 *
 *  XForms is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1, or
 *  (at your option) any later version.
 *
 *  XForms is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XForms.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef PTIMER_H_
#define PTIMER_H_


typedef struct
{
    double time_left;       /* the time (sec) left to wait */
    double timer;           /* total duration              */
    long sec,               /* start time                  */
         usec;
    int on,
        up;
    FL_TIMER_FILTER filter;
} FLI_TIMER_SPEC;


#define FLI_TIMER_BOXTYPE    FL_DOWN_BOX
#define FLI_TIMER_COL1       FL_COL1
#define FLI_TIMER_COL2       FL_RED
#define FLI_TIMER_LCOL       FL_LCOL
#define FLI_TIMER_ALIGN      FL_ALIGN_CENTER

#define FLI_TIMER_BLINKRATE  0.2


#endif /* PTIMER_H_ */


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
