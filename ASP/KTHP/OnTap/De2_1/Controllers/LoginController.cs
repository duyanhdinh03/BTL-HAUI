using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using De2_1.Models;

namespace De2_1.Controllers
{
    public class LoginController : Controller
    {
        private ModelDB db = new ModelDB();

        // GET: Login
        public ActionResult Index()
        {
            return View(db.Taikhoans.ToList());
        }
        public ActionResult Login()
        {
            return View();
        }

        [HttpPost]

        public ActionResult Login(string tdn, string password)
        {
            var taikhoan = db.Taikhoans.FirstOrDefault(n => n.tendn == tdn && n.matkhau == password);
            if (taikhoan != null)
            {
                Session["taikhoan"] = tdn;
                return RedirectToAction("Index" , "Management");
            }
            else
            {
                ViewBag.errMsg = "Tai khoan hoac mat khau khong dung";
                return View("Login");
            }
        }

        public ActionResult Logout()
        {
            Session["taikhoan"] = null;
            return RedirectToAction("Index", "Management");
        }
        // GET: Login/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Taikhoan taikhoan = db.Taikhoans.Find(id);
            if (taikhoan == null)
            {
                return HttpNotFound();
            }
            return View(taikhoan);
        }

        // GET: Login/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: Login/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "id,tendn,matkhau")] Taikhoan taikhoan)
        {
            if (ModelState.IsValid)
            {
                db.Taikhoans.Add(taikhoan);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(taikhoan);
        }

        // GET: Login/Edit/5
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Taikhoan taikhoan = db.Taikhoans.Find(id);
            if (taikhoan == null)
            {
                return HttpNotFound();
            }
            return View(taikhoan);
        }

        // POST: Login/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "id,tendn,matkhau")] Taikhoan taikhoan)
        {
            if (ModelState.IsValid)
            {
                db.Entry(taikhoan).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(taikhoan);
        }

        // GET: Login/Delete/5
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Taikhoan taikhoan = db.Taikhoans.Find(id);
            if (taikhoan == null)
            {
                return HttpNotFound();
            }
            return View(taikhoan);
        }

        // POST: Login/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            Taikhoan taikhoan = db.Taikhoans.Find(id);
            db.Taikhoans.Remove(taikhoan);
            db.SaveChanges();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}
