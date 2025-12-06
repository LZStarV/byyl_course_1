#include "TokenHeaderParser.h"

static void addChar(QSet<QChar>& set, QChar c)
{
    set.insert(c);
}

QSet<QChar> TokenHeaderParser::expandRanges(const QString& ranges)
{
    QSet<QChar> set;
    int         i      = 0;
    auto        n      = ranges.size();
    auto        skipWs = [&](void)
    {
        while (i < n && (ranges[i] == ' ' || ranges[i] == '\t')) i++;
    };
    skipWs();
    while (i < n)
    {
        skipWs();
        if (i >= n)
            break;
        QChar a = ranges[i++];
        if (i < n && ranges[i] == '-')
        {
            i++;
            if (i < n)
            {
                QChar  b  = ranges[i++];
                ushort ua = a.unicode(), ub = b.unicode();
                if (ua <= ub)
                {
                    for (ushort u = ua; u <= ub; ++u) addChar(set, QChar(u));
                }
                else
                {
                    for (ushort u = ub; u <= ua; ++u) addChar(set, QChar(u));
                }
            }
        }
        else
        {
            addChar(set, a);
        }
        skipWs();
        if (i < n && (ranges[i] == ',' || ranges[i] == ';'))
            i++;
        skipWs();
    }
    return set;
}

static bool matchFromSet(
    const QString& s, int& i, const QSet<QChar>& set, QString& out, bool atLeastOne)
{
    int start = i;
    while (i < s.size())
    {
        QChar c = s[i];
        if (!set.contains(c))
            break;
        out.append(c);
        i++;
    }
    return (i > start) || (!atLeastOne);
}

bool TokenHeaderParser::parseHeader(const QString&           s,
                                    const TokenHeaderConfig& cfg,
                                    int&                     code,
                                    bool&                    isGroup)
{
    code    = 0;
    isGroup = false;
    if (!cfg.prefix.isEmpty() && !s.startsWith(cfg.prefix))
        return false;
    int start = cfg.prefix.size();
    int end   = s.size();
    // handle optional group suffix at end
    if (!cfg.group_suffix.isEmpty() && end > start)
    {
        if (s[end - 1] == cfg.group_suffix[0])
        {
            isGroup = true;
            end -= 1;
        }
        else if (!cfg.group_suffix_optional)
        {
            return false;
        }
    }
    // scan digits from end backwards
    auto digitSet = expandRanges(cfg.code_digit_ranges);
    int  p        = end - 1;
    int  pow10    = 1;
    while (p >= start && digitSet.contains(s[p]))
    {
        int d = s[p].unicode() - '0';
        code += d * pow10;
        pow10 *= 10;
        p--;
    }
    if (pow10 == 1)
        return false;  // no digits
    int nameStart = start;
    int nameEnd   = p + 1;
    if (nameEnd <= nameStart)
        return false;  // empty name
    auto  firstSet = expandRanges(cfg.name_first_ranges);
    auto  restSet  = expandRanges(cfg.name_rest_ranges);
    QChar first    = s[nameStart];
    if (!firstSet.contains(first))
        return false;
    for (int k = nameStart + 1; k < nameEnd; ++k)
    {
        if (!restSet.contains(s[k]))
            return false;
    }
    return true;
}
