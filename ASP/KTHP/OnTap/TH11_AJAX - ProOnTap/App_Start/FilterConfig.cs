using System.Web;
using System.Web.Mvc;

namespace TH11_AJAX___ProOnTap
{
    public class FilterConfig
    {
        public static void RegisterGlobalFilters(GlobalFilterCollection filters)
        {
            filters.Add(new HandleErrorAttribute());
        }
    }
}
